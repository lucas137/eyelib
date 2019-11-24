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

#include "window/gaze_widget.hpp"

#include <utl/color.hpp>            // utl::color_rgb
#include <utl/fltk/fltk_draw.hpp>   // utl::fltk::draw_sector

namespace {   //-------------------------------------------------------------

using Color = utl::color_rgb;

// Raw Gaze
constexpr Color     raw_color{64,64,64};      // Dark gray
constexpr unsigned  raw_rad{4};               // Raw gaze radius in pixels
constexpr unsigned  raw_line{0};              // Raw gaze line width in pixels

// Smoothed Gaze
constexpr Color     avg_color_def{0,128,192}; // Default: light blue
constexpr Color     avg_color_fix{0,255,128}; // Fixation: light green
constexpr unsigned  avg_rad{18};              // Smoothed radius in pixels
constexpr unsigned  avg_line{3};              // Smoothed line width in pixels

} // anonymous --------------------------------------------------------------

namespace eye { namespace window {

GazeWidget::GazeWidget(eye::Gaze const& g)
: raw_x(g.raw_px.x)
, raw_y(g.raw_px.y)
, avg_x(g.avg_px.x)
, avg_y(g.avg_px.y)
, fixation(g.fixation)
{}

void
GazeWidget::draw() const
{
  namespace uc = utl::fltk::circle;
  if (show_raw)
  {
    uc::draw_circle<raw_rad, raw_line>(raw_x, raw_y, raw_color);
  }
  if (show_avg)
  {
    uc::draw_circle<avg_rad, avg_line>(
        avg_x, avg_y, (fixation ? avg_color_fix : avg_color_def));
  }
}

void
GazeWidget::set(eye::Gaze const& g)
{
  raw_x    = g.raw_px.x;
  raw_y    = g.raw_px.y;
  avg_x    = g.avg_px.x;
  avg_y    = g.avg_px.y;
  fixation = g.fixation;
}

} } // eye::window
//===========================================================================//
