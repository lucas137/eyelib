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
/// @brief    Window target widget.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYLIB_WINDOW_TARGET_WIDGET_HPP
#define EYLIB_WINDOW_TARGET_WIDGET_HPP

#include <eyelib.hpp>

#include <vector>   // std::vector

namespace eye { namespace window {

/// @ingroup    window
/// @{

 /// Visual target.
struct TargetWidget
{
  /// Construct with specified values.
  TargetWidget(unsigned x_px, unsigned y_px, bool active = false);

  TargetWidget(eye::Target const& t);   ///< Construct from object.
  TargetWidget() = default;             ///< Construct with default values.

  void active(bool a);    ///< `true` for active, `false` for inactive.

 //#define DYNAMIC_TARGET_WIDGET
 #ifdef DYNAMIC_TARGET_WIDGET
  void draw();            ///< Draw target.
  #else
  void draw() const;      ///< Draw target.
  #endif

private:
  int  x_{0};
  int  y_{0};
  bool active_{false};

 #ifdef DYNAMIC_TARGET_WIDGET
  unsigned  draw_count_{0};
  unsigned  dyn_radius_{8};
  bool      incr_radius_{false};  // True to increase radius, false to decrease
 #endif
};

/// Target container.
using TargetWidgets = std::vector<TargetWidget>;

#ifdef DYNAMIC_TARGET_WIDGET
/// Draw all widgets in container.
inline void
draw(TargetWidgets& targets)
{
  if (!targets.empty())
  {
    for (auto& t : targets)
    {
      t.draw();   // draw target(s)
    }
  }
}
#else
/// Draw all widgets in container.
inline void
draw(TargetWidgets const& targets)
{
  if (!targets.empty())
  {
    for (auto const& t : targets)
    {
      t.draw();   // draw target(s)
    }
  }
}
#endif

/// @}

} } // eye::window

#endif // EYLIB_WINDOW_TARGET_WIDGET_HPP
//===========================================================================//
