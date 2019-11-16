//===========================================================================//
//  Nathan Lucas
//  2015-2016
//===========================================================================//

#include "window/circle.hpp"

#include <FL/Fl.H>        // FLTK GUI libraries
#include <FL/fl_draw.H>   // fl_arc, fl_color, fl_line_style, fl_pie

namespace eye { namespace window {

//-----------------------------------------------------------

Circle::Circle(
    bool active, ColorRGB color_active, ColorRGB color_inactive,
    bool fill, unsigned line_px, bool show, unsigned radius_px,
    unsigned x_px, unsigned y_px)
: active(active)
, color_active(color_active)
, color_inactive(color_inactive)
, fill(fill)
, line_px(line_px)
, show(show)
, r_(radius_px)
, d_(radius_px * 2)
, bx_(x_px - radius_px)
, by_(y_px - radius_px)
{}

//-----------------------------------------------------------

void
Circle::draw() const
{
  ColorRGB col = (active ? color_active : color_inactive);
  fl_color(col.r, col.g, col.b);            // set line and fill color
  if (fill)
  {
    fl_pie(bx_, by_, d_, d_, 0.0, 360.0);   // filled circle
  }
  else
  {
    fl_line_style(FL_SOLID, line_px);       // set line width
    fl_arc(bx_, by_, d_, d_, 0.0, 360.0);   // draw circle
    fl_line_style(0);                       // restore default line width
  }
}

unsigned
Circle::radius_px() const
{
  return r_;
}

void
Circle::radius_px(unsigned val)
{
  r_ = val;
  d_ = val * 2;
}

void
Circle::center_px(int x, int y)
{
  x_px(x);
  y_px(y);
}

int  Circle::x_px() const  { return (bx_ + r_); }
void Circle::x_px(int val)  { bx_ = (val - r_); }
int  Circle::y_px() const  { return (by_ + r_); }
void Circle::y_px(int val)  { by_ = (val - r_); }

//-----------------------------------------------------------

} } // eye::window
//===========================================================================//
