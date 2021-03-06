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

#include "window/target_widget.hpp"

#include <utl/color.hpp>            // utl::color_rgb
#include <utl/fltk/fltk_draw.hpp>   // utl::fltk::draw_sector

namespace {   //-------------------------------------------------------------

using Color = utl::color_rgb;

constexpr Color disk_active{240, 240, 240};    // Light gray
constexpr Color disk_inactive{240, 240, 240};  // Light gray

constexpr Color dot_active{0, 0, 0};          // Black
constexpr Color dot_inactive{192, 192, 192};  // Gray

constexpr unsigned disk_radius_px{14};
constexpr unsigned dot_radius_px{3};

#ifdef DYNAMIC_TARGET_WIDGET
// Dynamic circle
constexpr unsigned dyn_circle_line_width    = 1;
constexpr unsigned dyn_circle_cycle         = 3;
constexpr unsigned dyn_circle_radius_min    = 1;
constexpr unsigned dyn_circle_radius_max    = 8;
#endif

} // anonymous --------------------------------------------------------------

namespace eye { namespace window {

TargetWidget::TargetWidget(unsigned x_px, unsigned y_px, bool active)
: x_(x_px)
, y_(y_px)
, active_(active)
{}

TargetWidget::TargetWidget(Target const& t)
: TargetWidget(t.x_px, t.y_px, t.active)
{}

void
TargetWidget::active(bool a)
{
  active_ = a;
}


void
#ifdef DYNAMIC_TARGET_WIDGET
TargetWidget::draw() //const
#else
TargetWidget::draw() const
#endif
{
  // Draw two filled circles with the same center point
  utl::fltk::circle::draw_disk<disk_radius_px>(x_, y_,
      active_ ? disk_active : disk_inactive);
  utl::fltk::circle::draw_disk<dot_radius_px>(x_, y_,
      active_ ? dot_active : dot_inactive);

  //utl::fltk::circle::draw_circle<56>(x_, y_, dot_active);

 #ifdef DYNAMIC_TARGET_WIDGET
  if (active_)
  {
    fl_color(0, 0, 0);              // set line color
    fl_line_style(FL_SOLID, dyn_circle_line_width);     // set line width
    fl_arc(x_ - dyn_radius_, y_ - dyn_radius_,
           2 * dyn_radius_, 2 * dyn_radius_, 0.0, 360.0);   // draw a circle
    fl_line_style(0);               // restore default line width (0)

    if (++draw_count_ == dyn_circle_cycle)
    {
      draw_count_ = 0;
      if (incr_radius_)
      {
        if (++dyn_radius_ > dyn_circle_radius_max) { incr_radius_ = false; }
      }
      else
      {
        if (--dyn_radius_ < dyn_circle_radius_min) { incr_radius_ = true; }
      }
    }
  }
 #endif
}

} } // eye::window
//===========================================================================//
