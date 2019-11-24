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
/// @brief    Eye tracker calibration eye data.
/// @author   Nathan Lucas
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
