//===========================================================================//
//  Nathan Lucas
//  2016
//===========================================================================//

#include "gaze/gaze_target.hpp"

#include <utl/randomize.hpp>

#include <thread>       // std::thread, std::this_thread::sleep_for
#include <chrono>       // std::chrono::milliseconds
#include <iostream>     // std::cout
#include <string>       // std::to_string

namespace eye {

//---------------------------------------------------------------------------
// public

GazeTarget::GazeTarget()
: call_target([](Target const& t){})     // Do-nothing callback
, call_targets([](Targets const& t){})   // Do-nothing callback
{}

GazeTarget::~GazeTarget()
{
  std::lock_guard<std::mutex> lock(mutex_);   // Acquire lock on mutex
  running_ = false;                           // Update flag
}

//-----------------------------------------------------------

void
GazeTarget::register_window(Window& win, bool event)
{
  std::lock_guard<std::mutex> lock(mutex_);     // Acquire lock on mutex
  if (event)
  {
    win.register_handler([this](Window::Event const& e){ handle(e); });
  }
  // Callbacks
  call_target  = [&win](Target const& t){ win.set(t); };
  call_targets = [&win](Targets const& t){ win.set(t); };
}

void
GazeTarget::set_targets(Targets const& t, TargetDuration const& delay_ms)
{
  {
    std::lock_guard<std::mutex> lock(mutex_);     // Acquire lock on mutex
    targets_  = t;
    index_    = t.size();
    delay_    = delay_ms;
  }
  reset();
}

bool
GazeTarget::get_target(Target& t) const
{
  std::lock_guard<std::mutex> lock(mutex_);   // Acquire lock on mutex
  if (!running_ || targets_.empty())
  {
    return false;
  }
  if (index_ < targets_.size())
  {
    t = targets_[index_];
  }
  return true;
}

bool
GazeTarget::is_started() const
{
  std::lock_guard<std::mutex> lock(mutex_);   // Acquire lock on mutex
  return running_;
}

//-----------------------------------------------------------

void
GazeTarget::start()
{
  start([this](){ point_start(); });
}

void
GazeTarget::start(handler callback)
{
  std::lock_guard<std::mutex> lock(mutex_);   // Acquire lock on mutex

  if (targets_.empty() || running_)
  {
    return;
  }
  running_ = true;
  call_targets(Targets());    // Hide all targets on screen

  // Asynchronous delay to show blank screen but also
  // allow the window to handle events and update accordingly
  schedule_callback(1000, [this, callback]()
    {
      std::lock_guard<std::mutex> lock(mutex_);   // Acquire lock on mutex
      index_ = 0;
      targets_[index_].active = false;
      call_target(targets_[index_]);
      schedule_callback(delay_.before_ms, callback);
    });
}

//-----------------------------------------------------------

void
GazeTarget::point_start()
{
  point_start([this](){ point_end(); });
}

void
GazeTarget::point_start(handler callback)
{
  {                                             // Create scope and
    std::lock_guard<std::mutex> lock(mutex_);   // acquire lock on mutex
    if (!targets_.empty() && running_)
    {
      targets_[index_].active = true;
      call_target(targets_[index_]);
      schedule_callback(delay_.active_ms, callback);
      return;     // return before reset
    }
  }           // Release lock upon leaving scope
  reset();    // Stop sequence
}

//-----------------------------------------------------------

void
GazeTarget::point_end()
{
  point_end([this](){ advance(); });
}

void
GazeTarget::point_end(handler callback)
{
  {                                             // Create scope and
    std::lock_guard<std::mutex> lock(mutex_);   // acquire lock on mutex
    if (!targets_.empty() && running_)
    {
      targets_[index_].active = false;
      call_target(targets_[index_]);
      schedule_callback(delay_.after_ms, callback);
      return;     // Return before reset
    }
  }           // Release lock upon leaving scope
  reset();    // Stop sequence
}

//-----------------------------------------------------------

void
GazeTarget::advance()
{
  advance([this](){ point_start(); });
}

void
GazeTarget::advance(handler callback)
{
  {                                             // Create scope and
    std::lock_guard<std::mutex> lock(mutex_);   // acquire lock on mutex
    if (!targets_.empty() && running_)
    {
      // Increment index and check if sequence completed
      if ((++index_) != targets_.size())
      {
        targets_[index_].active = false;
        call_target(targets_[index_]);
        schedule_callback(delay_.before_ms, callback);
        return;     // Return before reset
      }
    }
  }           // Release lock upon leaving scope
  reset();    // Stop sequence
}

//-----------------------------------------------------------

void
GazeTarget::reset()
{
  std::lock_guard<std::mutex> lock(mutex_);   // Acquire lock on mutex
  running_ = false;
  index_   = targets_.size();
  if (targets_.empty())
  {
    return;
  }
  for (auto& t : targets_)
  {
    t.active = true;      // Make all targets active
  }
  call_targets(targets_);     // Show all targets
  utl::randomize(targets_);
}

//---------------------------------------------------------------------------
// private

void
GazeTarget::handle(Window::Event const& e)
{
  using Special = eye::Window::Event::Key::Special;

  if (e.key.is_press())
  {
    if (e.key.is_enter())   // Enter key press
    {
      std::cout << (std::to_string(e.time_ms) + ",start_targets\n");
      start();
    }
    else if (e.key.special() == Special::escape)    // Esc key press
    {
      {
        std::lock_guard<std::mutex> lock(mutex_);   // Scoped lock on mutex
        if (!running_) { return; }                  // Verify if running_
      }
      std::cout << (std::to_string(e.time_ms) + ",stop_targets\n");
      reset();
    }
  }
}

void
GazeTarget::schedule_callback(unsigned delay_ms, handler callback)
{
  std::thread([this, delay_ms, callback]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
      if (running_)
      {
        callback();
      }
      else
      {
        reset();
      }
    }).detach();
}

//---------------------------------------------------------------------------

} // eye
//===========================================================================//
