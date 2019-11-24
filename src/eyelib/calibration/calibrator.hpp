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
/// @brief    Eye tracker calibrator.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYELIB_CALIBRATOR_HPP
#define EYELIB_CALIBRATOR_HPP

#include <eyelib/screen.hpp>

#include "gaze/gaze_target.hpp"
#include "tracker/message.hpp"
#include "window/window.hpp"

#include <utl/asio/tcp/client.hpp>  // utl::io::tcp::server

//#include <mutex>      // std::mutex, std::lock_guard

namespace eye { namespace tracker {

/// Eye tracker calibrator.
///
class Calibrator
{
public:

  Calibrator(utl::io::tcp::client& tcp);

  void setup(Window& win, Targets const& points,
             TargetDuration const& target_ms);

  void process_response(Message const& m);

private:
  GazeTarget  gaze_target_{};     // sequence of targets
  utl::io::tcp::client& tcp_;
};

} } // eye::tracker

#endif // EYELIB_CALIBRATOR_HPP
//===========================================================================//
