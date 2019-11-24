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
/// @brief    Window gaze point.
/// @author   Nathan Lucas
//===========================================================================//
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
