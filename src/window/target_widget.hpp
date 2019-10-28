//=============================================================================
/// @file
/// @brief    Window target widget.
/// @author   Nathan Lucas
/// @date     2016
//=============================================================================
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
