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

#include "tracker/message.hpp"

#include <eyelib/screen.hpp>   // eye::Screen

#include <utl/json.hpp>   // nlohmann::json

#include <iostream>   // std::cout, std::cerr
#include <string>     // std::string
#include <vector>     // std::vector
#include <exception>  // std::exception

namespace {   //-------------------------------------------------------------

void
calibration()
{
  namespace m = eye::tracker::message;

  std::cout << "----------------------------------------------------"
    <<'\n'<< "Calibration requests" << '\n'
    <<'\n'<< "calibration_start(9) : "    << m::calibration_start(9).dump(2)
    <<'\n'<< "calibration_point_start(100, 200) : "
                                << m::calibration_point_start(100, 200).dump(2)
    <<'\n'<< "calibration_point_end() : " << m::calibration_point_end().dump(2)
    <<'\n'<< "calibration_abort() : "     << m::calibration_abort().dump(2)
    <<'\n'<< "calibration_clear() : "     << m::calibration_clear().dump(2)
    <<'\n'<< "calibration_result() : "    << m::calibration_result().dump(2)
    <<'\n'<< "calibration_state() : "     << m::calibration_state().dump(2)
    <<'\n';
}

void
ostream_string()
{
  std::cout << "----------------------------------------------------"
    << '\n' << "Output stream and string conversion" << '\n'
    << '\n' << "eye::tracker::Message::Category::calibration : "
            <<  eye::tracker::Message::Category::calibration
    << '\n' << "eye::tracker::Message::Request::abort        : "
            <<  eye::tracker::Message::Request::abort
    << '\n' << "eye::tracker::Message::Status::bad_request   : "
            <<  eye::tracker::Message::Status::bad_request
    << std::endl;
}

void
predefined()
{
  namespace m = eye::tracker::message;
  using     j = nlohmann::json;
  try
  {
  std::cout << "----------------------------------------------------"
      <<'\n'<< "Pre-defined messages"<< '\n'
      <<'\n'<< "GET_CALIBRATION : "  << j::parse(m::GET_CALIBRATION).dump(2)
      <<'\n'<< "GET_DEVICE_STATE : " << j::parse(m::GET_DEVICE_STATE).dump(2)
      <<'\n'<< "GET_GAZE_DATA : "    << j::parse(m::GET_GAZE_DATA).dump(2)
      <<'\n'<< "GET_SCREEN : "       << j::parse(m::GET_SCREEN).dump(2)
      <<'\n'<< "REQUEST_CONNECT : "  << j::parse(m::REQUEST_CONNECT).dump(2)
      <<'\n'<< "REQUEST_HEARTBEAT : "<< j::parse(m::REQUEST_HEARTBEAT).dump(2)
      <<'\n';
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void
requests()
{
  namespace m = eye::tracker::message;
  using     v = eye::tracker::Message::Value;

  std::vector<std::string> keys{"push", "iscalibrated"};

  auto set_vals = m::set(nlohmann::json{
      { v::push_mode, true },
      { v::version, 1 }
    });

  eye::Screen scr(1, 1600, 0, 1920, 1200, 0.508, 0.3175);

  std::cout << "----------------------------------------------------"
    <<'\n'<< "Requests to server" << '\n'
    <<'\n'<< "get(\"push\")  : "                     << m::get("push").dump(2)
    <<'\n'<< "get(std::vector<std::string{ keys }) : " << m::get(keys).dump(2)
    <<'\n'<< "set(\"push\", true) :"           << m::set("push", true).dump(2)
    <<'\n'<< "set(nlohmann::json{ {key, val}, ... })"      << set_vals.dump(2)
    <<'\n'<< "set(Screen{ ... })"                       << m::set(scr).dump(2)
    <<'\n';
}

} // anonymous --------------------------------------------------------------

namespace eye { namespace tracker { namespace message { namespace debug {

void
message_test(TestMessage const& t)
{
  std::cout <<'\n'<< "eyelib: Test messages" <<'\n'<<'\n';
  try
  {
    switch (t)
    {
      case TestMessage::all:
        calibration();
        ostream_string();
        predefined();
        requests();
        break;
      case TestMessage::calibration:      calibration();      break;
      case TestMessage::ostream_string:   ostream_string();   break;
      case TestMessage::predefined:       predefined();       break;
      case TestMessage::requests:         requests();         break;
      default:
        break;  // invalid
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  std::cout << "----------------------------------------------------" << '\n';
}

} } } } // eye::tracker::message::debug
//===========================================================================//
