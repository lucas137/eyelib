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

#include "test_screen.hpp"
#include "eyelib_test.hpp"

#include <eyelib.hpp>

#include <utl/app.hpp>      // utl::app::key_wait()

#include <chrono>     // std::chrono::milliseconds
#include <thread>     // std::thread
#include <exception>  // std::exception
#include <iostream>   // std::cout

namespace {   //-------------------------------------------------------------

//// Free function gaze handler.
//void
//tracker_gaze(eye::Gaze const& g)
//{
//  std::cout << g << std::endl;
//}

// Application instructions.
void
app_instruct()
{
  std::cout << eye::test::line
    << '\n' << "Eyelib"
    << '\n' << "Press 'c' to open calibration window."
    << '\n' << "Press 'g' to open gaze point window."
    << '\n' << "Press 't' to open target sequence window."
    << '\n' << "Press Esc to exit."
    << '\n' << eye::test::line << '\n';
}

} // anonymous --------------------------------------------------------------

namespace eye { namespace test {

void
tracker(unsigned screen_index)
{
  std::cout << eye::test::line <<'\n'<< "eyelib: Test tracker" <<'\n';

  // Screen parameters
  auto scr = eye::screen(screen_index);
  std::cout << eye::screen_list(screen_index);

 #if 0
  int cx = scr.w_px/2;    // center of screen
  int cy = scr.h_px/2;
  eye::Targets points {               // "manually" define targets
      { cx - 100, cy - 100, true },
      { cx + 100, cy - 100, true },
      { cx - 100, cy + 100, true },
      { cx + 100, cy + 100, true }
    };
 #elif 1
  // Uniform distribution targets
  auto points = eye::target_array(scr, 5, 4);
  //auto points = eye::target_array(scr, 3, 3);
 #else
  // Uniform distribution with defined margin
  auto points = eye::target_array(scr, 3, 3, 120, 120);
 #endif

  // Calibration/target point milliseconds (before, during, after)
  eye::TargetDuration durations_ms{250, 1000, 250};

  try
  {
    eye::Tracker tracker("127.0.0.1", "6555", scr);
    //tracker.register_handler(tracker_gaze);
    std::cout << "start..." << '\n';
    tracker.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    app_instruct();

    std::thread loop_thread([&tracker, points, durations_ms]{
        int key = 0;
        while (key != 27)   // Loop until Esc key
        {
          key = utl::app::key_wait(200);
          switch (static_cast<char>(key))
          {
            case 'c':   // Calibrate
              tracker.calibrate(points, durations_ms);
              break;
            case 'g':   // Gaze window
              tracker.window();
              break;
            case 't':   // Target window
              tracker.window(points, durations_ms);
              break;
            default:
              break;
          }
        }
      });
    loop_thread.join();   // Block until thread finishes

    std::cout << "exit" << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

} } // eye::test
//===========================================================================//
