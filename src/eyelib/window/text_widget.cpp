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

#include "window/text_widget.hpp"

#include <FL/fl_draw.H>     // text color and font

#include <algorithm>  // std::max
#include <iostream>   // std::cerr
#include <exception>  // std::exception, std::out_of_range

namespace {   //-------------------------------------------------------------

constexpr Fl_Color  text_color  = FL_WHITE; //FL_BLACK;
constexpr Fl_Font   font_face   = FL_HELVETICA; //FL_COURIER;
constexpr int       font_size   = 20;             // in pixels
constexpr int       y_offset    = ((5 * font_size) / 4);

constexpr auto line1 = " •  Right click for options";
constexpr auto line2 = " •  Press Enter to start";
constexpr auto line3 = " •  Press Esc to cancel";
constexpr auto line4 = " •  Press Alt + F4 to exit";

int
width_px(std::string const& str)
{
  int dx = 0; // Offset of first "colored in" pixel of str, from draw origin.
  int dy = 0;
  int w = 0;  // Dimensions of the bounding box around the text.
  int h = 0;
  fl_font(font_face, font_size);  // Set text font prior to getting dimensions
  fl_text_extents(str.c_str(), dx, dy, w, h); // Min pixel dimensions of str
  return w;     // Return text width in pixels
}

} // anonymous --------------------------------------------------------------


namespace eye { namespace window {

//-----------------------------------------------------------

TextLine::TextLine(std::string const& str, int x_px, int y_px)
: str(str), x_px(x_px), y_px(y_px)
{}

//-----------------------------------------------------------

// x_px: (scr.w_px/2)
// y_px: (scr.h_px/3)
TextWidget::TextWidget(int x_px, int y_px, std::string const& intro)
: cx_(x_px)
, lines_{{
          { intro.c_str(), x_px, y_px - 2*y_offset  },
          { line1,         x_px, y_px - y_offset    },
          { line2,         x_px, y_px               },
          { line3,         x_px, y_px + y_offset    },
          { line4,         x_px, y_px + 2*y_offset  }}}
{
  int max_width_px = 0;
  for (auto const& l : lines_)
  {
    max_width_px = std::max(width_px(l.str), max_width_px);
  }
  // Determine horizontal position of the beginning of the text to center-align.
  int w = (max_width_px / 2);
  for (auto& l : lines_)
  {
    l.x_px -= w;
  }
}

void
TextWidget::draw() const
{
  if (show)
  {
    fl_color(text_color);           // set text color
    fl_font(font_face, font_size);  // set font
    for (auto const& t : lines_)    // draw each line of text
    {
      fl_draw(t.str.c_str(), t.x_px, t.y_px);
    }
  }
}

//-----------------------------------------------------------

} } // eye::window

//===========================================================================//
