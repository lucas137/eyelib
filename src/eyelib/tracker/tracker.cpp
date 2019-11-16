//===========================================================================//
//  Nathan Lucas
//  2016
//===========================================================================//

#include <eyelib.hpp>

#include "calibration/calibrator.hpp"
#include "debug/debug_out.hpp"
#include "gaze/gaze_target.hpp"
#include "tracker/message.hpp"
#include "window/window.hpp"

#include <utl/asio/tcp/client.hpp>  // utl::io::tcp::server
#include <utl/compile.hpp>          // utl::compile::date_yyyymmdd
#include <utl/json.hpp>             // nlohmann::json
#include <utl/memory.hpp>           // utl::make_unique
#include <utl/string.hpp>           // utl::parse

//#include <asio.hpp>   // Asio library

#include <chrono>     // std::chrono::milliseconds
#include <exception>  // std::exception
#include <string>     // std::string
#include <vector>     // std::vector
#include <iostream>   // std::cout

#include <atomic>     // std::atomic
#include <thread>     // std::thread
#include <mutex>      // std::mutex, std::lock_guard

//#define EYELIB_DEBUG
//#define EYELIB_HEARTBEAT

namespace {   //-------------------------------------------------------------
namespace msg = eye::tracker::message;
using     Msg = eye::tracker::Message;
} // anonymous --------------------------------------------------------------

namespace eye {


/////////////////////////////////////////////////////////////////////////////
// Tracker Implementation
/////////////////////////////////////////////////////////////////////////////

struct Tracker::Impl
{
  Screen          screen_{};          // screen parameters
  Tracker::State  state_{};           // tracker state data

  calib_handler   call_calib_handler;   // calibration results callback
  gaze_handler    call_gaze_handler;    // gaze data callback
  state_handler   call_state_handler;   // tracker state callback

  std::atomic<unsigned> gaze_time_ms_{0};   // timestamp of last gaze data
  mutable std::mutex    mutex_;             // mutual exclusion
 #ifdef EYELIB_HEARTBEAT
  std::thread           heartbeat_thread_;  // sends periodic heartbeat
 #endif
  std::thread           tcp_thread_;        // asynchronous read and write
  utl::io::tcp::client  tcp_;               // connection to device server

  tracker::Calibrator   calibrator_;        // eye tracker calibration
  GazeTarget            gaze_target_{};     // sequence of targets

  Impl(std::string const& host, std::string const& port, Screen const& scr);
  ~Impl();

 #ifdef EYELIB_HEARTBEAT
  void start_heartbeat(unsigned interval_ms);
 #endif
  void calibrate(Window& win, Targets const& points,
                 TargetDuration const& target_ms);

  void handle_read(std::string const& str);
  void process_calib_response(tracker::Message const& m);
  void process_tracker_response(tracker::Message const& m);
};

//---------------------------------------------------------------------------

Tracker::Impl::Impl(std::string const& host, std::string const& port,
                    Screen const& scr)
: screen_(scr)
, call_calib_handler([](eye::Calibration const&){})     // do-nothing callback
, call_gaze_handler([](eye::Gaze const&){})             // do-nothing callback
, call_state_handler([](eye::Tracker::State const&){})  // do-nothing callback
, mutex_()
#ifdef EYELIB_HEARTBEAT
, heartbeat_thread_()
#endif
, tcp_thread_()
, tcp_(host, port, std::bind(&handle_read, this, std::placeholders::_1))
, calibrator_(tcp_)
{}

Tracker::Impl::~Impl()
{
  // Create scope within which to acquire lock on mutex and update flag
  {
    std::lock_guard<std::mutex> lock(mutex_);
    state_.is_started = false;
  }                                     // Release lock upon leaving scope
  // Synchronize threads
  try
  {
   #ifdef EYELIB_HEARTBEAT
    if (heartbeat_thread_.joinable())
    {
      heartbeat_thread_.join();         // Wait for thread to finish
    }
   #endif
    tcp_.stop();                 // Close connection and stop client
    if (tcp_thread_.joinable())
    {
      tcp_thread_.join();        // Wait for thread to finish
    }
  }
  catch (std::exception& e)     // Catch and report standard exceptions
  {
    eye::debug::error(__FILE__, __LINE__, "Tracker::~Tracker(): ", e.what());
  }
  catch (...)                   // Catch and report all other exceptions
  {
    eye::debug::error(__FILE__, __LINE__,
                      "Tracker::~Tracker(): Unknown exception");
  }
}

//---------------------------------------------------------------------------
#ifdef EYELIB_HEARTBEAT
void
Tracker::Impl::start_heartbeat(unsigned interval_ms)
{
  heartbeat_thread_ = std::thread(
      [this, interval_ms]   // Lambda to send period heartbeat to server
      {
        bool loop = true;
        std::unique_lock<std::mutex> lock(mutex_);
        while (loop)
        {
          tcp_.write(msg::REQUEST_HEARTBEAT);
          lock.unlock();
          std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
          lock.lock();
          loop = state_.is_started;
        }
      });
}
#endif
//---------------------------------------------------------------------------

// Should we wait for a response after each request?
// No.  It's the caller's responsibility to confirm successful request.
// Exception:  Connection request.  The Client object itself is the
// caller, and is thus responsible for confirming successful request.

void
Tracker::Impl::handle_read(std::string const& str)
{
  std::unique_lock<std::mutex> lock(mutex_);  // acquire scoped lock on mutex

  if (!state_.is_started) { return; }

  //-----------------------------------------------------------
  // If str contains multiple messages, they must be parsed and processed one
  // at a time.  Otherwise, the JSON parser will process str as a single JSON
  // object, and throw an exception for missing ',' tokens between elements.
  std::vector<std::string> strings = utl::parse(str, '\n');

  for (auto const& s : strings)
  {
    //--------------------------------------
    // Deserialize and parse message.
    Msg message;
    if (!msg::parse(s, message))
    {                       // if error occurred while parsing,
      continue;             // stop processing and jump to next message
    }
    //--------------------------------------
    // Check message status code.
    switch (message.status)
    {
      // Custom notifications.
      case Msg::Status::calib_change:         // calibration changed
        //#########################################################################
        std::cout << gaze_time_ms_ << ",eyelib,GET_CALIBRATION\n";
        //#########################################################################
        tcp_.write(msg::GET_CALIBRATION);     // request state and results
        continue;
      case Msg::Status::screen_change:        // screen parameters changed
        tcp_.write(msg::GET_SCREEN);          // request parameters
        continue;
      case Msg::Status::device_change:        // device state changed
        tcp_.write(msg::GET_DEVICE_STATE);    // request state
        continue;
      // Relevant standard HTTP status codes.
      case Msg::Status::ok:   break;    // OK
      case Msg::Status::bad_request:    // bad request
      case Msg::Status::server_error:   // server error
      // Unrecognized code.
      default:
        eye::debug::error(__FILE__, __LINE__,
          "received unrecognized status code", message.json.dump(2));
        continue;
    }
    // Process message based on category.
    switch (message.category)
    {
      case Msg::Category::calibration:
        calibrator_.process_response(message);
        continue;
      case Msg::Category::tracker:
        lock.unlock();
        process_tracker_response(message);
        lock.lock();
        continue;
      case Msg::Category::heartbeat:
        continue;  // ignore
      default:
        eye::debug::error(__FILE__, __LINE__,
          "received unknown message category", message.json.dump(2));
        continue;
    }
  }
  //-----------------------------------------------------------
}

//---------------------------------------------------------------------------

void
Tracker::Impl::process_tracker_response(tracker::Message const& m)
{
  std::unique_lock<std::mutex> lock(mutex_);  // Acquire scoped lock on mutex

  // Assumes:
  //  m.category == msg::Category::TRACKER
  //  m.statuscode == 200 (OK)
  switch (m.request)
  {
    //-----------------------------------------------------------
    case Msg::Request::get:   // Successful "get" request
    {
      //--------------------------------------
      // Includes gaze data pushed by server.
      if (m.has_value(Msg::Value::gaze_data))
      {
        eye::Gaze g;
        if (msg::parse(m, g))
        {
          gaze_time_ms_ = g.time_ms;
          lock.unlock();
          call_gaze_handler(g);   // Invoke gaze data callback
          lock.lock();
        }
      }
     #ifdef EYELIB_DEBUG
      else
      {
        std::cout << "eyelib: successful \"get\" request:"
                  <<'\n'<< m.json.dump(2) <<'\n';
      }
     #endif
      //--------------------------------------
      // Calibration result
      if (m.has_value(Msg::Value::calibration_result))
      {
        eye::Calibration cal{};
        if (eye::tracker::message::parse(m, cal))
        {
         #ifdef EYELIB_DEBUG
          std::cout << "eyelib: calibresult:" <<'\n'<< cal <<'\n';
         #endif
          lock.unlock();
          call_calib_handler(cal);    // Invoke calibration result callback
          lock.lock();
        }
      }
      //--------------------------------------
      // Screen parameters
      //if (try_update(m, screen))
      if (m.values.count(Msg::Value::screen_index))
      {
       #ifdef EYELIB_DEBUG
        std::cout << "eyelib: screen" <<'\n';
        // TODO?...
       #endif
      }
      //--------------------------------------
      // Tracker state
      if (msg::try_update(m, state_))
      {
       #ifdef EYELIB_DEBUG
        std::cout << "eyelib: received tracker state" <<'\n';
       #endif
        lock.unlock();
        call_state_handler(state_);
        lock.lock();
      }
      //--------------------------------------
      break;
    }
    //-----------------------------------------------------------
    case Msg::Request::set:   // successful "set" request
    {
     #ifdef EYELIB_DEBUG
      std::cout << "eyelib: successful \"set\" request:"
                <<'\n'<< m.json.dump(2) <<'\n';
     #endif
      // First response from server?
      if (!state_.is_connected)
      {
        state_.is_connected = true;

        // Request state values.
        tcp_.write(msg::GET_TRACKER_STATE);
        tcp_.write(msg::GET_CALIBRATION);

        // Request set screen parameters.
        auto set_screen = msg::set(screen_);
       #ifdef EYELIB_DEBUG
        std::cout << "eyelib: set screen:" <<'\n'<< set_screen.dump(2) <<'\n';
       #endif
        tcp_.write(set_screen.dump());
      }
      break;
    }
    //-----------------------------------------------------------
    default:
      eye::debug::error(__FILE__, __LINE__,
        "received invalid message", m.json.dump(2));
      break;
    //-----------------------------------------------------------
  }
}


/////////////////////////////////////////////////////////////////////////////
// Tracker Class
/////////////////////////////////////////////////////////////////////////////

Tracker::Tracker(std::string const& host, std::string const& port,
                 Screen const& scr)
: pimpl(utl::make_unique<Impl>(host, port, scr))
{
  std::cout << "eyelib " << version
            << " (" << utl::compile::date_yyyymmdd_str("-") << ")"
            << std::endl;
}

Tracker::~Tracker()
{
  std::cout << "eyelib: exit" << std::endl;
}

//---------------------------------------------------------------------------

void
Tracker::register_handler(calib_handler callback)
{
  std::lock_guard<std::mutex> lock(pimpl->mutex_);  // Acquire lock on mutex
  if (callback)
  {
    pimpl->call_calib_handler = callback;           // Assign callback
  }
}

void
Tracker::register_handler(gaze_handler callback)
{
  std::lock_guard<std::mutex> lock(pimpl->mutex_);  // Acquire lock on mutex
  if (callback)
  {
    pimpl->call_gaze_handler = callback;            // Assign callback
  }
}

void
Tracker::register_handler(state_handler callback)
{
  std::lock_guard<std::mutex> lock(pimpl->mutex_);  // Acquire lock on mutex
  if (callback)
  {
    pimpl->call_state_handler = callback;           // Assign callback
  }
}

Tracker::calib_handler
Tracker::get_calib_handler() const  { return pimpl->call_calib_handler; }

Tracker::gaze_handler
Tracker::get_gaze_handler() const   { return pimpl->call_gaze_handler; }

Tracker::state_handler
Tracker::get_state_handler() const  { return pimpl->call_state_handler; }

//---------------------------------------------------------------------------

unsigned
Tracker::gaze_time_ms() const
{
  return pimpl->gaze_time_ms_;
}

Tracker::State
Tracker::state() const
{
  std::lock_guard<std::mutex> lock(pimpl->mutex_);  // Acquire lock on mutex
  return pimpl->state_;                             // Return tracker state
}

bool
Tracker::target(Target& t) const
{
  return pimpl->gaze_target_.get_target(t);
}

//---------------------------------------------------------------------------

void
Tracker::start(unsigned wait_ms)
{
  // Acquire scoped lock on mutex.
  std::unique_lock<std::mutex> lock(pimpl->mutex_);

  if(pimpl->state_.is_started) { return; } // Return if already running

  // Run pimpl->tcp_ in its own thread so it operates
  // asynchronously with respect to the rest of the program.
  pimpl->tcp_thread_ = std::thread([this](){ pimpl->tcp_.run(); });

  // Wait a little bit for connection.
  std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));

  // Request connection with tracker server.
  pimpl->tcp_.write(msg::REQUEST_CONNECT);

  pimpl->state_.is_started = true;
 #ifdef EYELIB_HEARTBEAT
  //pimpl->start_heartbeat(heartbeat_ms);
  pimpl->start_heartbeat(200);
 #endif
  lock.unlock();
  pimpl->call_state_handler(pimpl->state_);   // Invoke tracker state callback
}

void
Tracker::calibrate(Targets const& points, TargetDuration const& target_ms,
                   ColorRGB const& background)
{
  std::unique_lock<std::mutex> lock(pimpl->mutex_);
  Window win(*this, pimpl->screen_, "Eye Tracker Calibration", background);
  pimpl->calibrator_.setup(win, points, target_ms);
  lock.unlock();
  win.run();      // Blocks until window is closed
}

void
Tracker::window(ColorRGB const& background)
{
  std::unique_lock<std::mutex> lock(pimpl->mutex_);
  Window win(*this, pimpl->screen_, "Eye Tracker Gaze", background);
  win.show_avg_gaze(true);
  win.show_raw_gaze(true);
  lock.unlock();
  win.run();      // Blocks until window is closed
}

void
Tracker::window(Targets const& points, TargetDuration const& target_ms,
                ColorRGB const& background)
{
  if (points.empty())
  {
    window(background);   // If no target points, open gaze window instead
    return;
  }
  std::unique_lock<std::mutex> lock(pimpl->mutex_);
  Window win(*this, pimpl->screen_, "Eye Tracker Targets", background);
  pimpl->gaze_target_.register_window(win);
  pimpl->gaze_target_.set_targets(points, target_ms);
  lock.unlock();
  win.run();      // Blocks until window is closed
}


/////////////////////////////////////////////////////////////////////////////

} // eye
//===========================================================================//
