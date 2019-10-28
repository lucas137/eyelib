//=============================================================================
/// @file
/// @brief    Window gaze point.
/// @author   Nathan Lucas
/// @date     2016
//=============================================================================
#ifndef EYLIB_WINDOW_GAZE_WIDGET_HPP
#define EYLIB_WINDOW_GAZE_WIDGET_HPP

#include <eyelib/gaze.hpp>  // eye::Gaze

namespace eye { namespace window {

/// @ingroup    window
/// @addtogroup text  Window Gaze Point
/// @{

/// Gaze point widget.
struct GazeWidget
{
  int  raw_x{0};    ///< Raw gaze X coordinate.
  int  raw_y{0};    ///< Raw gaze Y coordinate.
  int  avg_x{0};    ///< Smoothed gaze X coordinate.
  int  avg_y{0};    ///< Smoothed gaze Y coordinate.

  bool fixation{false};   ///< `true` for fixation point.
  bool show_raw{false};   ///< `true` to draw raw gaze point.
  bool show_avg{false};   ///< `true` to draw smoothed gaze point.

  GazeWidget(eye::Gaze const& g);   ///< Construct from gaze data.
  GazeWidget() = default;           ///< Construct with default values.

  void draw() const;              ///< Draw gaze raw point and/or smoothed.
  void set(eye::Gaze const& g);   ///< Set gaze data.
};

/// @}

} } // eye::window

#endif // EYLIB_WINDOW_GAZE_WIDGET_HPP
//===========================================================================//
