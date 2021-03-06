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
/// @brief    Eye tracker calibration point.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYELIB_CALIB_POINT_HPP
#define EYELIB_CALIB_POINT_HPP

#include "calibration/calib_eyes.hpp"

#include <sstream>    // std::ostringstream
#include <ostream>    // std::ostream
#include <iomanip>    // std::setfill
#include <string>     // std::string

namespace eye {

//---------------------------------------------------------------------------

/// Convert integral value to sample state.
inline Calibration::Point::State
calib_sample_state(unsigned n)
{
  using S = Calibration::Point::State;
  return ((n == 0) ? S::no_data :
          (n == 1) ? S::resample :
          (n == 2) ? S::ok :
         /*default*/ S::error);
}


inline std::string
to_string(Calibration::Point::State const& s)
{
  using S = Calibration::Point::State;
  return ((s == S::no_data)  ? "no_data" :
          (s == S::resample) ? "resample" :
          (s == S::ok)       ? "ok" :
          /*default*/          "error");
}

inline std::ostream&
operator<<(std::ostream& os, Calibration::Point::State const& s)
{
  return os << eye::to_string(s);
}

//---------------------------------------------------------------------------

/*
  sample      calibrate    estimated    accuracy   mean_err   std_dev
  state       (pixel)      (pixel)      (degree)   (pixel)    (pixel)
  resample    0000,0000    0000,0000    00.0000    00.0000    00.0000
*/

inline std::string
to_string(Calibration::Point const& p)
{
  std::ostringstream oss;
  oss << std::left      << std::setfill(' ')
      << std::setw(8)   << p.sample_state
      << std::right     << std::fixed
      << std::setprecision(0)
      << std::setw(8)   << p.calibrate_px.x << ','
      << std::setw(4)   << p.calibrate_px.y
      << std::setw(8)   << p.estimated_px.x << ','
      << std::setw(4)   << p.estimated_px.y
      << std::setprecision(4)
      << std::setw(11)  << p.accuracy_deg.binocular
      << std::setw(11)  << p.mean_error_px.binocular
      << std::setw(11)  << p.avg_std_dev_px.binocular;
  return oss.str();
}

inline std::ostream&
operator<<(std::ostream& os, Calibration::Point const& p)
{
  return os << eye::to_string(p);
}

inline std::string
csv(Calibration::Point const& p)
{
  std::ostringstream oss;
  oss <<        p.sample_state
      << ',' << p.calibrate_px.x << ',' << p.calibrate_px.y
      << ',' << p.estimated_px.x << ',' << p.estimated_px.y
      << ',' << csv(p.accuracy_deg)
      << ',' << csv(p.accuracy_rating)
      << ',' << csv(p.mean_error_px)
      << ',' << csv(p.avg_std_dev_px)
      ;
  return oss.str();
}

#if 0
/// @ingroup  eyelib_gaze
/// @brief    Convert to JSON object.
inline nlohmann::json
json(Calibration::Point const& p)
{
  return { { "sample_state", p.sample_state },
           { "calibrate_px",
             { "x", p.calibrate_x_px },
             { "y", p.calibrate_y_px } },
           { "estimated_px",
             { "x", p.estimated_x_px },
             { "y", p.estimated_y_px } },
           { "accuracy_deg", p.accuracy_deg },
           { "mean_error_px", p.mean_error_px },
           { "avg_std_dev_px", p.avg_std_dev_px }
         };
}
#endif

//---------------------------------------------------------------------------

} // eye

#endif // EYELIB_CALIB_POINT_HPP
//===========================================================================//
