//=============================================================================
/// @file
/// @brief    Window calibration point widget.
/// @author   Nathan Lucas
/// @date     2016
//=============================================================================
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
