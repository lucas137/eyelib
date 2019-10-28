//===========================================================================//
//  Nathan Lucas
//  2015-2016
//===========================================================================//

#include "window/calib_widget.hpp"

#include <utl/color.hpp>            // utl::color_rgb
#include <utl/fltk/fltk_draw.hpp>   // utl::fltk::draw_sector

namespace {   //-------------------------------------------------------------

static constexpr utl::color_rgb gray   { 127, 127, 127 };  ///< Uncalibrated
static constexpr utl::color_rgb red    { 224,   0,   0 };  ///< Recalibrate
static constexpr utl::color_rgb orange { 224, 128,   0 };  ///< Poor
static constexpr utl::color_rgb yellow { 255, 255,   0 };  ///< Moderate
static constexpr utl::color_rgb lime   { 127, 224,   0 };  ///< Good
static constexpr utl::color_rgb green  {   0, 160,   0 };  ///< Great

static constexpr int  radius_px { 19 };       ///< Radius in pixels.

utl::color_rgb
rating_color(eye::Calibration::Rating const& r)
{
  using R = eye::Calibration::Rating;
  return ((r == R::recalibrate)  ? red :
          (r == R::poor)         ? orange :
          (r == R::moderate)     ? yellow :
          (r == R::good)         ? lime :
          (r == R::great)        ? green :
          /*default*/              gray);
}

} // anonymous --------------------------------------------------------------

namespace eye { namespace window {

CalibPointWidget::CalibPointWidget(
    int x_px, int y_px, utl::color_rgb const& top,
    utl::color_rgb const& left, utl::color_rgb const& right)
: x_(x_px)
, y_(y_px)
, color_top(top)
, color_left(left)
, color_right(left)
{}

CalibPointWidget::CalibPointWidget(Calibration::Point const& p)
: x_(p.calibrate_px.x)
, y_(p.calibrate_px.y)
, color_top( rating_color(p.accuracy_rating.binocular) )
, color_left( rating_color(p.accuracy_rating.left_eye) )
, color_right( rating_color(p.accuracy_rating.right_eye) )
{}

void
CalibPointWidget::draw() const
{
  namespace c = utl::fltk::circle;
  // Draw three 120 degree circular sectors with the same center point
  c::draw_sector<radius_px,  30, 150>(x_, y_,color_top);
  c::draw_sector<radius_px, 150, 270>(x_, y_,color_right);
  c::draw_sector<radius_px, 270, 390>(x_, y_,color_left);
}

//---------------------------------------------------------------------------

void
CalibWidget::draw() const
{
  using S = CalibWidget::Show;
  switch (show_)
  {
    case S::average:
      average_.draw();
      break;
    case S::points:
      if (!points_.empty())
      {
        for (auto const& p : points_)
        {
          p.draw();     // draw calibration point results
        }
      }
      break;
    case S::none:
    default:
      break;  // Invalid!
  }
}

void
CalibWidget::set(Calibration const& c)
{
  // Range construct from container c.points
  points_ = window::CalibPointWidgets(c.points.cbegin(), c.points.cend());

  // Compute centroid of the calibration points
  unsigned cx = 0;
  unsigned cy = 0;
  eye::centroid(cx, cy, c.points);

  // Get colors that correspond to the average error ratings
  auto color_top   = rating_color(c.error_rating.binocular);
  auto color_left  = rating_color(c.error_rating.left_eye);
  auto color_right = rating_color(c.error_rating.right_eye);

  // Construct from values
  average_ = window::CalibPointWidget(cx, cy, color_top, color_left, color_right);
}

CalibWidget::Show
CalibWidget::show() const
{
  return show_;
}

void
CalibWidget::show(Show const& s)
{
  show_ = s;
}

//---------------------------------------------------------------------------
} } // eye::window
//===========================================================================//
