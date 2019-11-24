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
/// @brief    Eye tracker window.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYELIB_WINDOW_WINDOW_HPP
#define EYELIB_WINDOW_WINDOW_HPP

#include <eyelib.hpp>

#include "window/event.hpp"

#include <vector>     // std::vector
#include <string>     // std::string
#include <ostream>    // std::ostream
#include <functional> // std::function
#include <memory>     // std::unique_ptr

namespace eye {

struct Screen;
class Tracker;

/// @addtogroup eyelib_window
/// The Window class supports visualization of gaze targets,
/// raw and smoothed gaze points, and eye tracker calibration.
/// @{

//---------------------------------------------------------------------------

/** @brief  Visualization window

### Instantiate

  ```
  eye::Screen scr = eye::screen(1);           // Screen parameters
  eye::ColorRGB bg{149, 149, 149};            // Optional background color
  std::string title("Eye Tracker Window");    // Window title
  Window win(scr, title, bg);                 // Instantiate window
  ```

### Register Handler
  Clients can register to receive state changes notifications
  by passing a callable object to `register_handler()`.
  ```
  void handler_function(eye::Window::State const& s) { … }      // Callback function

  win.register_handler(handler_function);                       // Register callback

  win.register_handler([](eye::Window::State const& s){ … });   // Register lambda expression
  ```
  Example:
  ```
  win.register_handler([](eye::Window::State const& s){
        std::cout << "window state: " << s << '\n';
      });
  ```

### Targets
  One or more targets can be displayed on the screen by
  passing a `Target` object or `Targets` container to `set()`.
  Calling `clear_targets()` removes all targets from the screen.
  ```
  // Display a single target point on the screen
  eye::Target target{960, 600, true};   // Single target
  win.set(target);

  // Display multiple target points on the screen
  eye::Targets targets {
      { cx,       cy,       true  },
      { cx - 100, cy - 100, false },
      { cx + 100, cy - 100, false },
      { cx - 100, cy + 100, false },
      { cx + 100, cy + 100, false }
    };
  win.set(targets);

  // Remove all targets from the screen
  win.clear_targets();
  ```

### %Gaze Points
  Clients can specify whether to show smoothed and/or raw gaze points on the
  screen by calling `show_avg_gaze()` and `show_raw_gaze()` respectively.
  ```
  win.show_avg_gaze(true);    // Display smoothed gaze point on the screen
  win.show_raw_gaze(true);    // Display raw gaze point on the screen
  ```

### Run %Window
  The last step to create a window is to call `run()`.  The window registers
  itself to receive calibration results and streaming gaze data from the
  specified `Tracker`, updates its state, issues a state change notification
  to the `State` handler (if one is [registered](@ref register_handler)),
  and enters a run loop until the window is closed.  This is a blocking call,
  so it is important the client complete any setup, start threads, etc.
  before invoking `run()`.
  ```
  eye::Tracker tracker("127.0.0.1", "6555", scr);
  …
  win.run(tracker);   // Blocks until window is closed
  ```
*/
class Window
{
public:
  //-----------------------------------------------------------

  /// Window state.
  enum class State
  {
    init,     ///< Initializing
    ready,    ///< Running - showing title screen
    active,   ///< Running - showing main screen
    close,    ///< Closing
  };

  using Event = window::Event;

  /// Event handler alias.
  using event_handler = std::function<void(Event const&)>;

  /// State change notification handler alias.
  using state_handler = std::function<void(State const&)>;

  //-----------------------------------------------------------

  /// @brief  Construct window.
  /// @param  [in]  tracker Eye tracker.
  /// @param  [in]  scr     Screen parameters.
  /// @param  [in]  title   Window title.
  /// @param  [in]  bg      Background color.
  explicit                            // direct initialization only
  Window(Tracker& tracker, Screen const& scr,
         std::string const& title, ColorRGB const& bg = {149,149,149});

  ~Window();                                  ///< Destructor.
  Window(Window const&)            = delete;  ///< Prohibit copying.
  Window& operator=(Window const&) = delete;  ///< Prohibit assignment.

  /// @brief Run window.
  ///
  /// The window registers itself to receive gaze data from @a tracker.
  /// @note This call blocks until the window is closed.
  int run();

  /// Mark window as needing to be redrawn.
  void redraw() const;

  //-----------------------------------------------------------
  /// @name Handler registration
  /// @{

  void register_handler(event_handler callback);  ///< %Window event handler.
  void register_handler(state_handler callback);  ///< %Window state handler.

  /// @}
  //-----------------------------------------------------------
  /// @name Set properties
  /// @{

  void clear_targets();           ///< Remove all targets from screen.
  void set(Target const& t);      ///< %Target to draw on screen.
  void set(Targets const& ts);    ///< %Targets to draw on screen.

  //void show_calib(bool val);      ///< `true` to draw calibration results.
  void show_avg_gaze(bool val);   ///< `true` to draw raw gaze point.
  void show_raw_gaze(bool val);   ///< `true` to draw smoothed gaze point.

  /// @}
  //-----------------------------------------------------------

private:
  struct Impl;                    // Implementation struct
  std::unique_ptr<Impl> pimpl;    // Pointer to implementation
};

//---------------------------------------------------------------------------

/// @brief    Convert to string.
/// @ingroup  eyelib_window
inline std::string
to_string(Window::State const& val)
{
  switch (val)
  {
    case Window::State::init:   return "init";
    case Window::State::ready:  return "ready";
    case Window::State::active: return "active";
    case Window::State::close:  return "close";
    default:                    return "error";
  }
}

/// @name     Non-member function overloads
/// @relates  eye::Window::State
/// @{

/// Insert into output stream.
inline std::ostream&
operator<<(std::ostream& os, Window::State const& val)
{
  return os << to_string(val);
}

/// @}

//---------------------------------------------------------------------------

/// @}

} // eye

#endif // EYELIB_WINDOW_WINDOW_HPP
//===========================================================================//
