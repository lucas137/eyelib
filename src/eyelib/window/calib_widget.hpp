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
/// @brief    Window calibration point widget.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYLIB_WINDOW_CALIB_WIDGET_HPP
#define EYLIB_WINDOW_CALIB_WIDGET_HPP

#include <eyelib/calibration.hpp>

#include <utl/color.hpp>  // utl::color_rgb

#include <vector>   // std::vector

namespace eye { namespace window {

/// @ingroup    window
/// @{

//---------------------------------------------------------------------------
/// @brief  Calibration point widget.
///
/// Draws three circular sectors of 120 degrees.
/// The color of each sector is set independently.
class CalibPointWidget
{
public:

  /// Construct with specified values.
  CalibPointWidget(int x_px, int y_px,
              utl::color_rgb const& top,
              utl::color_rgb const& left,
              utl::color_rgb const& right);

  CalibPointWidget(Calibration::Point const& p);   ///< Construct from Point.
  CalibPointWidget() = default;                    ///< Default constructor.

  /// Draw widget.
  void draw() const;

private:
  int  x_{0};
  int  y_{0};
  utl::color_rgb  color_top   {127,127,127};    ///< Upper sector.
  utl::color_rgb  color_left  {127,127,127};    ///< Lower-left.
  utl::color_rgb  color_right {127,127,127};    ///< Lower-right.
};

/// Target container.
using CalibPointWidgets = std::vector<CalibPointWidget>;

//---------------------------------------------------------------------------

/// Average calibration error, or accuracy of each calibration point.
class CalibWidget
{
public:
  enum class Show
  {
    average,
    points,
    none,
  };

  void draw() const;                ///< Draw calibration point widget(s).
  void set(Calibration const& c);   ///< Set calibration error values.
  Show show() const;                ///< Get results to be drawn.
  void show(Show const& s);         ///< Set results to be drawn.

private:
  CalibPointWidget   average_{};
  CalibPointWidgets  points_{};
  Show               show_{Show::none};
};

//---------------------------------------------------------------------------
/// @}

} } // eye::window

#endif // EYLIB_WINDOW_CALIB_WIDGET_HPP
//===========================================================================//
