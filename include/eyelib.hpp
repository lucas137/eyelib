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
/// @brief    Eyelib eye tracking library.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYELIB_HPP
#define EYELIB_HPP

#include <string>

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

constexpr auto version = "v1.0.2";  ///< Eyelib library version.

/// Returns a string containing the date of
/// compilation (e.g., `"20160312"`, `"2016-03-12"`)
std::string
build_date(std::string const& delim="");

/// Returns a string containing the time of
/// compilation (e.g., `"hhmmss"`, `"hh:mm:ss"`).
std::string
build_time(std::string const& delim="");

} // eye  -------------------------------------------------------------------

#include <eyelib/calibration.hpp>
#include <eyelib/gaze.hpp>
#include <eyelib/screen.hpp>
#include <eyelib/tracker.hpp>

#endif // EYELIB_HPP
//===========================================================================//
