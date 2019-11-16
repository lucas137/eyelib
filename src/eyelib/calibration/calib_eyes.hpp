//===========================================================================//
/// @file
/// @brief    Eye tracker calibration eye data.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYELIB_CALIB_EYEE_HPP
#define EYELIB_CALIB_EYES_HPP

#include <sstream>    // std::ostringstream
#include <ostream>    // std::ostream
#include <string>     // std::string

namespace eye {

//---------------------------------------------------------------------------

template<typename T>
inline std::string
to_string(Calibration::Eyes<T> const& e, std::string const& prefix = "")
{
  std::ostringstream oss;
  oss <<       prefix << "binocular : " << e.binocular
      <<'\n'<< prefix << "left_eye  : " << e.left_eye
      <<'\n'<< prefix << "right_eye : " << e.right_eye;
  return oss.str();
}

template<typename T>
inline std::ostream&
operator<<(std::ostream& os, Calibration::Eyes<T> const& e)
{
  return os << eye::to_string(e);
}

template<typename T>
inline std::string
csv(Calibration::Eyes<T> const& e)
{
  std::ostringstream oss;
  oss <<        e.binocular
      << ',' << e.left_eye
      << ',' << e.right_eye;
    return oss.str();
}

//---------------------------------------------------------------------------

} // eye

#endif // EYELIB_CALIB_EYES_HPP
//===========================================================================//
