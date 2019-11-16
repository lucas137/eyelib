//===========================================================================//
/// @file
/// @brief    Eye gaze target.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYELIB_GAZE_TARGET_HPP
#define EYELIB_GAZE_TARGET_HPP

#include "window/window.hpp"

#include <functional>   // std::function
#include <mutex>        // std::mutex, std::lock_guard

namespace eye {
//---------------------------------------------------------------------------

/**
  @brief  %Target sequence.

  member          | callback delay   | default callback
  ----------------|------------------|-----------------
  `start()`       | `delay_ms`       | `point_start()`
  `point_start()` | `duration_ms`    | `point_end()`
  `point_end()`   | `0` milliseconds | `advance()`
  `advance()`     | `delay_ms`       | `point_start()`
*/
class GazeTarget
{
public:

  explicit
  GazeTarget();       ///< Constructor.
  ~GazeTarget();      ///< Destructor.

  // std::mutex objects cannot be copied/moved.  Both the copy
  // constructor and assignment operators are automatically deleted.

  /// @brief  Register callbacks.
  /// @param  [in]  event   `true` to register for window event notifications.
  ///
  /// Assign callbacks to set window targets, and optionally
  /// register to receive notifications of window events.
  void register_window(Window& win, bool event = true);

  /// @brief  Set targets and timer delays.
  /// @param  [in]  t           Container of gaze targets.
  /// @param  [in]  delay_ms    Target delay times in milliseconds.
  void set_targets(Targets const& t,
                   TargetDuration const& delay_ms = {500,1000,500});

  /// @brief  Return the current target point.
  /// @return `true` if sequence started.  `false` if
  ///          not started or no targets have been set.
  ///
  /// If sequence is not started or no targets have
  /// been set, the value of @a t is not changed.
  bool get_target(Target& t) const;

  bool is_started() const;    ///< Returns `true` if sequence is started.

  using handler = std::function<void()>;    ///< For user defined callbacks.

  void start(handler callback);       ///< Start sequence, show first target.
  void start();                       ///< Default callback: point_start().

  void point_start(handler callback); ///< Start active state of point.
  void point_start();                 ///< Default callback: point_stop().

  void point_end(handler callback);   ///< Stop active state of point.
  void point_end();                   ///< Default callback: advance().

  void advance(handler callback);     ///< Advance to the next target.
  void advance();                     ///< Default callback: point_start().

  void reset();       ///< Cancel sequence, show all targets on screen.

private:

  // Callback to process window events
  void handle(Window::Event const& e);

  // Asynchronously invoke callback after delay in milliseconds
  void schedule_callback(unsigned delay_ms, handler callback);

  mutable std::mutex  mutex_{};
  eye::Targets        targets_{};
  std::size_t         index_{0};
  TargetDuration      delay_{500,1000,500};
  bool                running_{false};

  using target_handler  = std::function<void(Target const& t)>;  // callback
  using targets_handler = std::function<void(Targets const& t)>;  // callback
  target_handler      call_target;
  targets_handler     call_targets;

};

//---------------------------------------------------------------------------
#if 0
inline void
target_update(Window& win, Target& t, bool active)
{
  t.active = !t.active;
  win.set(t);
}

inline void
target_sequence(Window& win, Targets& points)
{
  unsigned sample_delay_ms    = 1000;
  unsigned sample_duration_ms = 1000;

  for (auto& p : points)
  {
    target_update(win, p, false);

    utl::delay_sync(sample_delay_ms,
      [&win, &p](){ target_update(win, p, true); });

    utl::delay_sync(sample_duration_ms,
      [&win, &p](){ target_update(win, p, false); });
  }
}
#endif

//---------------------------------------------------------------------------
} // eye

#endif // EYELIB_GAZE_TARGET_HPP
//===========================================================================//
