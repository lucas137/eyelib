//===========================================================================//
/*  MIT License

Copyright (c) 2019 Nathan Lucas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
//===========================================================================//
/// @file
/// @brief    Eye tracker manager.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYELIB_TRACKER_HPP
#define EYELIB_TRACKER_HPP

//#include <eyelib/calibration.hpp> // eye::Calibration
//#include <eyelib/gaze.hpp>        // eye::Gaze
//#include <eyelib/screen.hpp>      // eye::Screen

#include <functional> // std::function
#include <string>     // std::string
#include <memory>     // std::unique_ptr

namespace eye {

struct Calibration;
struct Gaze;
struct Screen;

/**
  @addtogroup eyelib_tracker

  <tt>\#include \<eyelib.hpp\></tt> @a -or- @n
  <tt>\#include \<eyelib/tracker.hpp\></tt>

  The Tracker class provides core eye tracker functionality.
*/
/// @{

// Doxygen note: Prepend labels with % to override automatic link generation.

/////////////////////////////////////////////////////////////////////////////
//  Tracker Class
/////////////////////////////////////////////////////////////////////////////

/**
###############################################################################
  @brief  Eye tracker manager

  - Connects to an eye tracker server.
  - Sends periodic heartbeat messages in order to maintain server connection.
  - Access to streaming gaze data
  - Access to state information:
    + Device hardware connection
    + Server connection
    + Streaming data frame rate
    + %Calibration status

### Instantiate   #############################################################

  Clients must supply a TCP host, a port number, and screen parameters to
  construct a `Tracker` object.  The necessary `Screen` object for a given
  display can be obtained by calling `eye::screen()` with the screen number.
  If no index is given, `eye::screen()` returns parameters for screen `0`.
  ```
  std::string host("127.0.0.1");                  // TCP host and
  std::string port("6555");                       // port number

  unsigned index = 1;                             // Screen number
  auto scr = eye::screen(index);           // Get screen parameters
  std::cout << eye::screen_list(index) << '\n';   // Optionally get screen list

  eye::Tracker tracker(host, port, scr);          // Instantiate tracker, or...
  …
  eye::Tracker tracker("127.0.0.1", "6555", eye::screen(1));  // In a single line, or...
  …
  eye::Tracker tracker("127.0.0.1", "6555", eye::screen());   // Default screen (0)
  ```
### Register Handlers   #######################################################

  Clients can register to receive streaming gaze data and/or state change
  notifications by passing a callable object to `register_handler()`.
  ```
  // Handler function
  void f(eye::Tracker::Calibration const& c)
  {
    // Do stuff
    …
  }

  // Register a function callback
  tracker.register_handler(f);

  // Register a lambda expression - do stuff between the brackets { … }
  tracker.register_handler([](eye::Gaze const& g){ … });

  // Unregister a callback by registering an empty closure
  tracker.register_handler([](eye::Tracker::State const&){});
  ```
  Member functions can be invoked within the body of a lambda expression.
  ```
  class GazeHandler
  {
  public:
    void handle(eye::Gaze const& g) { … }   // Callback to process gaze data
  };

  // Handler object
  GazeHandler obj;

  // Capture obj by reference and call member handle_gaze
  tracker.register_handler([&obj](eye::Gaze const& g){ obj.handle_gaze(g); });
  ```

### %Gaze Data   ##############################################################

  Example gaze data:
  ```
  Gaze g = {
      "2016-07-09 21:35:48.628",  // Timestamp string
      42969664,                   // Timestamp in milliseconds
      false,                      // Fixation flag
      0x07,                       // Tracking state
      981.062, 1387.65,           // Raw gaze point in pixels
      980.973, 1381.57,           // Smoothed gaze point in pixels
      22.4632, 24.1758            // Left and right pupil size
    };
  ```
### Start   ###################################################################

  Clients must call `start()` to connect to the eye tracker server in order to
  begin receiving gaze data.  The caller can specify wait time in milliseconds
  between starting asynchronous TCP I/O and requesting a server connection.
  ```
  tracker.start();        // Default wait time
  …
  tracker.start(100);     // 100 ms wait time
  ```
### Current %State   ##########################################################

  In addition to registering for state change notifications, clients can
  obtain current state information by calling `state()`.
  ```
  auto s = tracker.state();

  std::cout << s << '\n';         // Output to console

  switch (s.device_state) { … }   // Hardware device connnection state

  unsigned f = s.frame_rate;      // Gaze data frame rate

  if (is_calibrated) { … }        // true if tracker is calibrated
  if (is_calibrating) { … }       // true if being calibrated
  if (is_connected) { … }         // true if connected to server
  if (is_started) { … }           // true if tracker is started
  ```
### %Gaze Targets   ###########################################################

  ```
  // Define individual target points
  eye::Targets points {
    { cx - 100, cy - 100, true },
    { cx + 100, cy - 100, true },
    { cx - 100, cy + 100, true },
    { cx + 100, cy + 100, true }
  };

  // Generate uniformly distributed targets
  auto points = eye::target_array(scr, 3, 3);
  ```
### %Calibration   ############################################################

  Member `calibrate()` opens a window to calibrate the eye tracker at the
  specified points.  A call to `calibrate()` blocks until the window is closed.
  ```
  eye::Targets        points     = { … };
  eye::TargetDuration target_ms  = {500,1000,500};  // Before, during, after
  eye::ColorRGB       background = {149,149,149};   // Red, green, blue

  tracker.calibrate(points, target_ms, background);

  // List initialization
  tracker.calibrate(points, {500,1000,500}, {149,149,149});

  // Default window background color
  tracker.calibrate(points, {250, 500, 250});

  // Default target durations and background color
  tracker.calibrate(points);

  ```
### Window   ##################################################################

  Member `window()` opens a window to display gaze points and/or gaze targets.
  A call to `window()` blocks until the window is closed.
  ```
  eye::ColorRGB background = {149,149,149};   // Red, green, blue

  // Gaze window
  window(background);     // Specified background color
  window({149,149,149});  // Background can be list initialized
  window();               // Default background

  eye::Targets        points    = { … };
  eye::TargetDuration target_ms = {500,1000,500};  // Before, during, after

  // Gaze target window
  window(points, target_ms, background);  // Specified target durations and background
  window(points, target_ms);              // Default background color
  window(points);                         // Default durations and background

  // Target durations and background can be list initialized
  window(points, {500,1000,500}, {149,149,149});
  ```
###############################################################################
*/
//----------------------------------------------------------------------------
// Example:  Options to receive eye tracker streaming gaze data.
#if 0
  eye::Tracker  eye_tracker;
 #if 1
  // Lambda ---------------------------------------------------
  eye_tracker.register_handler(
    [](eye::Gaze const& g){ std::cout << g << '\n'; });
 #elif 0
  // Free function gaze handler -------------------------------
  inline void
  handle_gaze(eye::Gaze const& g)
  {
    std::cout << g << std::endl;
  }
  eye_tracker.register_handler(handle_gaze);
 #else
  // Class with gaze handler method ---------------------------
  class GazeHandler
  {
  public:
    // Gaze handler method
    inline void
    handle(eye::Gaze const& g)
    {
      std::cout << g << std::endl;
    }
  };
  GazeHandler gh;
  // Lambda to call handler method
  eye_tracker.register_handler([&gh](eye::Gaze const& g){ gh.handle(g); });
 #endif
  //-----------------------------------------------------------
  eye_tracker.start();
#endif
//----------------------------------------------------------------------------

class Tracker
{
public:

  //-----------------------------------------------------------

  /// Physical tracker device connection state.
  enum class Device : unsigned
  {
    connected     = 0,  ///< Device detected and working.
    not_connected = 1,  ///< Device not detected.
    bad_firmware  = 2,  ///< Device detected, but wrong/unsupported firmware.
    no_usb_three  = 3,  ///< Device detected, but unsupported USB host.
    no_stream     = 4,  ///< Device detected, but no stream could be received.
    unrecognized  = 5   ///< Unrecognized device state code.
  };

  /// Device, server, and calibration states.
  struct State
  {
    Device   device_state   = Device::not_connected; ///< Physical connnection.
    unsigned frame_rate     = 0;       ///< Gaze data frame rate.
    bool     is_calibrated  = false;   ///< `true` if tracker is calibrated.
    bool     is_calibrating = false;   ///< `true` if being calibrated.
    bool     is_connected   = false;   ///< `true` if connected to server.
    bool     is_started     = false;   ///< `true` if tracker is started.
  };

  /// Calibration change notification handler alias.
  using calib_handler = std::function<void(Calibration const&)>;

  /// Streaming gaze data handler alias.
  using gaze_handler  = std::function<void(Gaze const&)>;

  /// State change notification handler alias.
  using state_handler = std::function<void(State const&)>;

  //-----------------------------------------------------------

  /// @brief  Construct an eye tracker manager.
  /// @param  [in]  host  TCP address string.
  /// @param  [in]  port  Port number string.
  /// @param  [in]  scr   Screen parameters.
  explicit                              // direct initialization only
  Tracker(std::string const& host, std::string const& port,
          Screen const& scr);

  ~Tracker();                                   ///< Destructor.
  Tracker(Tracker const&)            = delete;  ///< Prohibit copying.
  Tracker& operator=(Tracker const&) = delete;  ///< Prohibit assignment.

  //-----------------------------------------------------------
  /// @name Handler registration
  /// @{

  /// Register to receive calibration change notifications via @a callback.
  void
  register_handler(calib_handler callback);

  /// Register to receive streaming gaze data via @a callback.
  void
  register_handler(gaze_handler callback);

  /// Register to receive state change notifications via @a callback.
  void
  register_handler(state_handler callback);

  /// Return the currently registered calibration callback.
  calib_handler
  get_calib_handler() const;

  /// Return the currently registered gaze data callback.
  gaze_handler
  get_gaze_handler() const;

  /// Return the currently registered state change callback.
  state_handler
  get_state_handler() const;

  /// @}
  //-----------------------------------------------------------
  /// @name Object inspection
  /// @{

  /// Return the timestamp in milliseconds of the last received gaze data.
  unsigned
  gaze_time_ms() const;

  /// Return current state.
  State
  state() const;

  ///// Get current gaze data.
  //Gaze
  //gaze() const;

  /// @brief  Return the current target.
  /// @return `true` if sequence started.  `false` if
  ///          not started or no targets have been set.
  ///
  /// If sequence is not started or no targets have
  /// been set, the value of @a t is not changed.
  bool
  target(Target& t) const;

  /// @}
  //-----------------------------------------------------------
  /// @name Operations
  /// @{

  /// @brief  Connect to an eye tracker.
  /// @param  [in]  wait_ms       Wait in milliseconds between starting
  ///         asynchronous TCP I/O and requesting a server connection.
  void
  start(unsigned wait_ms = 50);

  /// @brief  Calibrate at the specified points.
  /// @param  [in]  points      Calibration points.
  /// @param  [in]  target_ms   Target delay times in milliseconds.
  /// @param  [in]  background  Background color.
  /// @note   Blocks until the window is closed.
  void
  calibrate(Targets const& points,
            TargetDuration const& target_ms = {500,1000,500},
            ColorRGB const& background = {149,149,149});

  /// @brief  Open a gaze window
  /// @param  [in]  background  Background color.
  /// @note   Blocks until the window is closed.
  void
  window(ColorRGB const& background = {149,149,149});

  /// @brief  Open a target window
  /// @param  [in]  points      Target points.
  /// @param  [in]  target_ms   Target delay times in milliseconds.
  /// @param  [in]  background  Background color.
  ///
  /// Creates a window with a sequence of targets at the specified points.
  /// If no points are given, a gaze window is greated without targets.
  /// @note   Blocks until the window is closed.
  void
  window(Targets const& points,
         TargetDuration const& target_ms = {500,1000,500},
         ColorRGB const& background = {149,149,149});

  /// @}
  //-----------------------------------------------------------

private:
  struct Impl;                    // Implementation struct
  std::unique_ptr<Impl> pimpl;    // Pointer to implementation
};


/////////////////////////////////////////////////////////////////////////////
//  Device State
/////////////////////////////////////////////////////////////////////////////

/// Convert integer to device state.
Tracker::Device
tracker_device_state(unsigned code);

/// Convert to integer.
unsigned
to_number(Tracker::Device const& val);

/// Convert to string.
std::string
to_string(Tracker::Device const& val);

/// @name     Non-member function overloads
/// @relates  eye::Tracker
/// @{

/// @brief    Insert into output stream.
std::ostream&
operator<<(std::ostream& os, Tracker::Device const& val);

/// @}


/////////////////////////////////////////////////////////////////////////////
//  Tracker State
/////////////////////////////////////////////////////////////////////////////
/// @name     Non-member function overloads
/// @relates  eye::Tracker::State
/// @{

/// Test equivalence.
bool
operator==(Tracker::State const& lhs, Tracker::State const& rhs);

/// Test difference.
bool
operator!=(Tracker::State const& lhs, Tracker::State const& rhs);

/// Insert into output stream.
std::ostream&
operator<<(std::ostream& os, Tracker::State const& s);

/// @}
/////////////////////////////////////////////////////////////////////////////
//  Testing Only
/////////////////////////////////////////////////////////////////////////////
/// @internal
/// @{
/// Testing only.
namespace tracker { namespace message { namespace debug {

/// @internal
/// Eye tracker server test message types.
enum class TestMessage
{ all, calibration, ostream_string, predefined, requests };

/// @internal
/// Test eye tracker server messages.
void message_test(TestMessage const& t);

} } } // tracker::message::debug
/// @}
/////////////////////////////////////////////////////////////////////////////
/// @}
} // eye

#endif // EYELIB_TRACKER_HPP
//===========================================================================//
