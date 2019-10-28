//===========================================================================//
/// @file
/// @brief    Screen-related data structures and functions.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYELIB_SCREEN_HPP
#define EYELIB_SCREEN_HPP

#include <utl/color.hpp>

#include <string>     // std::string
#include <ostream>    // std::ostream
#include <vector>     // std::vector

namespace eye {

/**
  @addtogroup eyelib_screen

  <tt>\#include \<eyelib.hpp\></tt> @a -or- @n
  <tt>\#include \<eyelib/screen.hpp\></tt>

  %Screen-related data structures and functions.
*/
/// @{

//---------------------------------------------------------------------------

/// RGB color.
struct ColorRGB
{
  unsigned char r;    ///< Red.
  unsigned char g;    ///< Green.
  unsigned char b;    ///< Blue.
};

//---------------------------------------------------------------------------

/// 2D point.
template<typename T>
struct PointXY
{
  T x;    ///< X coordinate.
  T y;    ///< Y coordinate.
};

//---------------------------------------------------------------------------

/// Display parameters
struct Screen
{
  unsigned index = 0;     ///< Index of screen (for multiple displays).
  int      x_px  = 0;     ///< Horizontal position in pixels.
  int      y_px  = 0;     ///< Vertical position in pixels.
  unsigned w_px  = 0;     ///< Resolution width in pixels.
  unsigned h_px  = 0;     ///< Resolution height in pixels.
  float    w_m   = 0.0;   ///< Physical width in meters.
  float    h_m   = 0.0;   ///< Physical height in meters.

  /// Construct with specified values.
  Screen(unsigned index, int x_px, int y_px,
         unsigned w_px, unsigned h_px, float w_m, float h_m);

  /// Construct with zero values.
  Screen() = default;
};

//-----------------------------------------------------------

/// @name     Non-member function overloads
/// @relates  eye::Screen
/// @{

/// Test equivalence and return `true` if condition holds.
bool
operator==(Screen const& lhs, Screen const& rhs);


/// Test difference and return `true` if condition holds.
bool
operator!=(Screen const& lhs, Screen const& rhs);

/// Insert into output stream.
std::ostream&
operator<<(std::ostream& os, Screen const& s);

/// @}

//-----------------------------------------------------------

/** @ingroup  eyelib_screen
  @brief  Return a screen object with the specified index.

  If @a n does not identify a valid screen number, the
  object is default constructed with zero for all values.
*/
Screen
screen(unsigned index = 0);

/** @ingroup  eyelib_screen
  @brief  List postion and size of all screens, and
          indicate the screen associated with @a index.

  Example:
  ```
  [ ] screen 0 : 1600 x 900 (0, 0)
  [x] screen 1 : 1920 x 1200 (1600, 0)
  [ ] screen 2 : 1920 x 1200 (3520, 0)
  ```
*/
std::string
screen_list(unsigned index);

//---------------------------------------------------------------------------

/// Eye gaze target.
struct Target
{
  int  x_px;      ///< Center X coordinate in pixels.
  int  y_px;      ///< Center Y coordinate in pixels.
  bool active;    ///< `true` for active, `false` for inactive.
};

using Targets = std::vector<Target>;  ///< Target container.

//-----------------------------------------------------------

/// @name     Non-member function overloads
/// @relates  eye::Target
/// @{

/// Insert into output stream.
std::ostream&
operator<<(std::ostream& os, Target const& t);

/// @}

//-----------------------------------------------------------

/// @brief  Return an @a x by @a y array of targets evenly spaced over @a scr.
///
/// The outer-most targets are offset from the edge of the screen
/// by a margin equal to 10% of minimum screen dimension.
Targets
target_array(Screen const& scr, unsigned x, unsigned y);

/// @brief Return an @a x by @a y array of targets evenly spaced over @a scr.
///
/// The outer-most targets are offset from the edge of the screen
/// by @a margin_x and @a margin_y in pixels.
Targets
target_array(Screen const& scr, unsigned x, unsigned y,
             unsigned margin_x, unsigned margin_y);

//---------------------------------------------------------------------------

/// Eye gaze target duration and delay times.
struct TargetDuration
{
  unsigned before_ms;   ///< Delay in milliseconds before active state.
  unsigned active_ms;   ///< Duration in milliseconds of active state.
  unsigned after_ms;    ///< Delay in milliseconds after active state.
};

//---------------------------------------------------------------------------

/// @}

} // eye

#endif // EYELIB_SCREEN_HPP
//===========================================================================//
