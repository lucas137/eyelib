//=============================================================================
/// @file
/// @brief    Window circle widget.
/// @author   Nathan Lucas
/// @date     2015-2016
//=============================================================================
#ifndef EYLIB_WINDOW_CIRCLE_HPP
#define EYLIB_WINDOW_CIRCLE_HPP

#include <eyelib/window.hpp>     // eye::Window::ColorRGB

namespace eye { namespace window {

using ColorRGB = eye::Window::ColorRGB;

/// @ingroup    window
/// @{

struct Circle       ///< Circle widget.
{
  bool      active{false};      ///< `true` if target is active.
  ColorRGB  color_active{};     ///< RBG color when active.
  ColorRGB  color_inactive{};   ///< RBG color when inactive.
  bool      fill{false};        ///< `true` for solid fill circle.
  unsigned  line_px{0};         ///< Line width in pixels.
  bool      show{false};        ///< `true` to draw on screen.

  /// Construct from values.
  Circle(bool active, ColorRGB color_active, ColorRGB color_inactive,
         bool fill, unsigned line_px, bool show,
         unsigned radius_px, unsigned x_px, unsigned y_px);
  Circle() = default;                 ///< Default construct object.

  void     draw() const;              ///< Draw circle.
  unsigned radius_px() const;         ///< Get radius in pixels.
  void     radius_px(unsigned val);   ///< Set radius in pixels.
  void     center_px(int x, int y);   ///< Set center point in pixels.
  int      x_px() const;      ///< Get center X coordinate in pixels.
  void     x_px(int val);     ///< Set center X coordinate in pixels.
  int      y_px() const;      ///< Get center Y coordinate in pixels.
  void     y_px(int val);     ///< Set center Y coordinate in pixels.

private:
  unsigned  r_{0};        // radius in pixels
  unsigned  d_{r_ * 2};   // diameter in pixels
  int       bx_{0};       // horizonal position of bounding box in pixels
  int       by_{0};       // vertical position of bounding box in pixels
};

/// @}

} } // eye::window

#endif // EYLIB_WINDOW_CIRCLE_HPP
//===========================================================================//
