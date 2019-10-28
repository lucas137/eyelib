//===========================================================================//
/// @file
/// @brief    Eyelib eye tracking library.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYELIB_HPP
#define EYELIB_HPP
/**
  @namespace  eye
  @brief  <b>Eyelib</b> -- eye tracking library.

  @defgroup eyelib  eyelib
  @{
    @defgroup eyelib_calib    calib
    @defgroup eyelib_gaze     gaze
    @defgroup eyelib_screen   screen
    @defgroup eyelib_tracker  tracker
  @}
*/
namespace eye {   //---------------------------------------------------------

constexpr auto version = "v2.1";  ///< Eyelib library version.

} // eye  -------------------------------------------------------------------

#include <eyelib/calibration.hpp>
#include <eyelib/gaze.hpp>
#include <eyelib/screen.hpp>
#include <eyelib/tracker.hpp>

#endif // EYELIB_HPP
//===========================================================================//
