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

#include "test_gaze.hpp"
#include "eyelib_test.hpp"

#include <eyelib.hpp>

#include <utl/app.hpp>      // utl::app::key_wait()

#include <iostream>   // std::cout
#include <exception>  // std::exception
#include <thread>     // std::thread

namespace {   //-------------------------------------------------------------

// Free function gaze handler.
void
handle_gaze(eye::Gaze const& g)
{
  std::cout << g << std::endl;
}

// Class with gaze handler method.
class GazeHandler
{
public:
  // Gaze handler method.
  void
  handle(eye::Gaze const& g)
  {
    std::cout << g << std::endl;
  }
};

// Wait for Escape key in 200 millisecond loop.
void
wait_loop()
{
  // Wait for Escape key in 200 millisecond loop.
  // Lambda to call wait_key; equivalent:
  //    std::thread wait_thread(utl::app::wait_key, 27, 200);
  std::thread wait_thread([]{ utl::app::key_wait(27, 200); });
  wait_thread.join();       // Block until thread finishes
}

} // anonymous --------------------------------------------------------------

namespace eye { namespace test {

void
gaze_handler(unsigned screen_index, Handler const& handler_type)
{
  std::cout <<'\n'<< "eyelib: Test gaze" <<'\n';

  std::cout << eye::screen_list(screen_index) << '\n';
  try
  {
    eye::Tracker tracker("127.0.0.1", "6555", eye::screen(screen_index));

    switch (handler_type)
    {
      //---- Free function handler ----
      case Handler::function:

        tracker.register_handler(handle_gaze);
        break;

      //---- Lambda gaze handler ----
      case Handler::lambda:

        // Process data in closure of lambda expression
        tracker.register_handler(
            [](eye::Gaze const& g){ std::cout << g << '\n'; });
        break;

      //---- Class method gaze handler ----
      case Handler::member:
      default:

        GazeHandler gh;
        // Use lambda to call handler method
        tracker.register_handler([&gh](eye::Gaze const& g){ gh.handle(g); });
        break;
    }

    std::cout << "start..." << '\n' << eye::test::line <<'\n';

    tracker.start();
    wait_loop();

    std::cout << eye::test::line << '\n' << "exit" << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

} } // eye::test
//===========================================================================//
