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
/// @brief    Eye tracker calibration rating.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYELIB_CALIB_RATING_HPP
#define EYELIB_CALIB_RATING_HPP

#include <sstream>    // std::ostringstream
#include <ostream>    // std::ostream
#include <iomanip>    // std::setprecision
#include <string>     // std::string

namespace eye {

//---------------------------------------------------------------------------

constexpr double error_deg_great    = 0.5;
constexpr double error_deg_good     = 0.7;
constexpr double error_deg_moderate = 1.0;
constexpr double error_deg_poor     = 1.5;

/// Return rating based on average error in degrees.
inline Calibration::Rating
calib_rating(bool is_calibrated, float error_deg)
{
  using R = eye::Calibration::Rating;
  return (!is_calibrated                      ? R::uncalibrated
         : (error_deg <= error_deg_great)     ? R::great
         : (error_deg <= error_deg_good)      ? R::good
         : (error_deg <= error_deg_moderate)  ? R::moderate
         : (error_deg <= error_deg_poor)      ? R::poor
         : /*default*/                          R::recalibrate);
}

/// Return rating based on average error in degrees.
inline Calibration::Eyes<Calibration::Rating>
calib_rating(bool is_calibrated, Calibration::Eyes<float> error_deg)
{
  return { calib_rating(is_calibrated, error_deg.binocular),
           calib_rating(is_calibrated, error_deg.left_eye),
           calib_rating(is_calibrated, error_deg.right_eye) };
}

/// Return rating in the range `0`-`5` based on average error in degrees.
inline unsigned
calib_rating_num(bool is_calibrated, float error_deg)
{
  return static_cast<unsigned>(calib_rating(is_calibrated, error_deg));
}

/// @brief  Return a description of the five-star calibration rating.
/// @param  [in]  rating  Rating on a scale 0-5.
/// @param  [in]  deg     Degrees accuracy.
/// @return String containing a description of calibration rating.
inline std::string
calib_rating_str(bool is_calibrated, float error_deg)
{
  using std::to_string;
  std::string err = std::to_string(error_deg);
  switch (calib_rating_num(is_calibrated, error_deg))
  {
    case 0:  return "[     ] Uncalibrated";
    case 1:  return "[*    ] Recalibrate";
    case 2:  return "[**   ] ("+ err +" <= "+ to_string(error_deg_poor)     +" degree)  Poor";
    case 3:  return "[***  ] ("+ err +" <= "+ to_string(error_deg_moderate) +" degree)  Moderate";
    case 4:  return "[**** ] ("+ err +" <= "+ to_string(error_deg_good)     +" degree)  Good";
    case 5:  return "[*****] ("+ err +" <= "+ to_string(error_deg_great)    +" degree)  Great!";
    default: return "[ERROR]";
  }
}

inline std::string
to_string(Calibration::Rating const& r)
{
  using R = Calibration::Rating;
  return ((r == R::recalibrate) ? "recalibrate"
         : (r == R::poor)       ? "poor"
         : (r == R::moderate)   ? "moderate"
         : (r == R::good)       ? "good"
         : (r == R::great)      ? "great"
         : /*default*/            "uncalibrated");
}

inline std::ostream&
operator<<(std::ostream& os, Calibration::Rating const& r)
{
  return os << eye::to_string(r);
}

//---------------------------------------------------------------------------

} // eye

#endif // EYELIB_CALIB_RATING_HPP
//===========================================================================//
