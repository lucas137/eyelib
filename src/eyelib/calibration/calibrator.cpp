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
//  Nathan Lucas
//  2016
//===========================================================================//

#include "calibration/calibrator.hpp"

#include "debug/debug_out.hpp"  // eye::debug::error

#include <iostream>   // std::cout

namespace {   //-------------------------------------------------------------

namespace msg = eye::tracker::message;
using     Msg = eye::tracker::Message;

} // anonymous --------------------------------------------------------------

namespace eye { namespace tracker {

//---------------------------------------------------------------------------

Calibrator::Calibrator(utl::io::tcp::client& tcp)
: tcp_(tcp)
{}

//---------------------------------------------------------------------------

void
Calibrator::setup(Window& win, Targets const& points,
                  TargetDuration const& target_ms)
{
  // Register callbacks to send targets to the window, but do not
  // register gaze_target_ to receive notifications of window events.
  // Events will be handled in the lambda expression below.
  gaze_target_.register_window(win, false);

  // Set targets and timer delays.
  gaze_target_.set_targets(points, target_ms);

  // Register to receive window events.
  win.register_handler([this, points/*, &mutex*/](Window::Event const& e)
    {
      using Special = eye::Window::Event::Key::Special;

      // Need to lock when sending messages, but release
      // in order to receive responses from the server.
      //std::lock_guard<std::mutex> lock(mutex);

      // Main thread locked by FLTK; see Window::Impl::handle()

      if (e.key.is_press())   // Key press
      {
        if (e.key.is_enter())   // Enter key
        {
          if (!gaze_target_.is_started())
          {
            std::cout << (std::to_string(e.time_ms) + ",start_calibration\n");
            tcp_.write(msg::calibration_start(points.size()).dump());
          }
        }
        else if (e.key.special() == Special::escape)  // Esc key
        {
          // 2016-09-25 Lucas:  Must be able to abort calibration in the case
          //    of all points were calibrated but the calibration failed.
          //if (gaze_target_.is_started())
          //{
            std::cout << (std::to_string(e.time_ms) + ",abort_calibration\n");
            tcp_.write(msg::calibration_abort().dump());
          //}
        }
      }
    });
}

//---------------------------------------------------------------------------

void
Calibrator::process_response(Message const& m)
{
  // Assumes:
  //  m.category == msg::Category::CALIBRATION
  //  m.statuscode == 200 (OK)
  switch (m.request)
  {
    //-----------------------------------------------------------
    case Msg::Request::start:         // successful "start" request
    {
      gaze_target_.start([this]()
        {
          Target t;
          if (gaze_target_.get_target(t))
          {
            #ifdef EYELIB_DEBUG
            std::cout << "eyelib: calibration_point_start("
                      << t.x_px << ',' << t.y_px << ")\n";
            #endif
            tcp_.write(msg::calibration_point_start(t.x_px, t.y_px).dump());
          }
        });
      break;
    }
    //-----------------------------------------------------------
    case Msg::Request::point_start:   // successful "pointstart" request
    {
      gaze_target_.point_start([this]()
        {
          #ifdef EYELIB_DEBUG
          std::cout << "eyelib: calibration_point_end()\n";
          #endif
          tcp_.write(msg::calibration_point_end().dump());
        });
      break;
    }
    //-----------------------------------------------------------
    case Msg::Request::point_end:     // successful "pointend" request
    {
      gaze_target_.point_end([this]()
        {
          gaze_target_.advance([this]()
            {
              Target t;
              if (gaze_target_.get_target(t))
              {
                #ifdef EYELIB_DEBUG
                std::cout << "eyelib: calibration_point_start("
                          << t.x_px << ',' << t.y_px << ")\n";
                #endif
                tcp_.write(msg::calibration_point_start(t.x_px,t.y_px).dump());
              }
            });
        });

      // May include values with calibresult object...

      //if ( last point completed )
      //{
      //  // Evaluate calibresult object...
      //}

      break;
    }
    //-----------------------------------------------------------
    case Msg::Request::abort:         // successful "abort" request
    case Msg::Request::clear:         // successful "clear" request
      gaze_target_.reset();
      break;
    //-----------------------------------------------------------
    default:
      eye::debug::error(__FILE__, __LINE__,
        "received invalid message", m.json.dump(2));
      break;
    //-----------------------------------------------------------
  }
}

//---------------------------------------------------------------------------

} } // eye::tracker
//===========================================================================//
