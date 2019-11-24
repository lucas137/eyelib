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

#include <eyelib.hpp>

#include <tuple>      // std::tie
#include <string>     // std::string
#include <ostream>    // std::ostream

namespace eye {

/////////////////////////////////////////////////////////////////////////////
// Device State
/////////////////////////////////////////////////////////////////////////////

Tracker::Device
tracker_device_state(unsigned code)
{
  using D = Tracker::Device;
  return ((code == 0) ? D::connected :
          (code == 1) ? D::not_connected :
          (code == 2) ? D::bad_firmware :
          (code == 3) ? D::no_usb_three :
          (code == 4) ? D::no_stream :
                        D::unrecognized);
}

unsigned
to_number(Tracker::Device const& val)
{
  return static_cast<unsigned>(val);
}

std::string
to_string(Tracker::Device const& val)
{
  using D = Tracker::Device;
  switch (val)
  {
    case D::connected:     return "Connected";
    case D::not_connected: return "Not Connected";
    case D::bad_firmware:  return "Unsupported Firmware";
    case D::no_usb_three:  return "Unsupported USB";
    case D::no_stream:     return "No Stream Received";
    case D::unrecognized:
    default:               return "Unrecognized Code";
  }
}

std::ostream&
operator<<(std::ostream& os, Tracker::Device const& val)
{
  return os << eye::to_string(val);
}


/////////////////////////////////////////////////////////////////////////////
// Tracker State
/////////////////////////////////////////////////////////////////////////////

bool
operator==(Tracker::State const& lhs, Tracker::State const& rhs)
{
  return std::tie(lhs.device_state, lhs.frame_rate, lhs.is_calibrated,
                  lhs.is_calibrating, lhs.is_connected, lhs.is_started)
      == std::tie(rhs.device_state, rhs.frame_rate, rhs.is_calibrated,
                  rhs.is_calibrating, rhs.is_connected, rhs.is_started);
}

bool
operator!=(Tracker::State const& lhs, Tracker::State const& rhs)
{
  return !(lhs == rhs);
}

std::ostream&
operator<<(std::ostream& os, Tracker::State const& s)
{
  return os << "device_state   : " <<  s.device_state
    << '\n' << "frame_rate     : " <<  s.frame_rate
    << '\n' << "is_calibrated  : " << (s.is_calibrated  ? "true" : "false")
    << '\n' << "is_calibrating : " << (s.is_calibrating ? "true" : "false")
    << '\n' << "is_connected   : " << (s.is_connected   ? "true" : "false")
    << '\n' << "is_started     : " << (s.is_started     ? "true" : "false");
}


/////////////////////////////////////////////////////////////////////////////

} // eye
//===========================================================================//
