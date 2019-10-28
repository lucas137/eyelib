//===========================================================================//
/// @file
/// @brief    Test eye tracker window.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYELIB_TEST_WINDOW_HPP
#define EYELIB_TEST_WINDOW_HPP

#include <eyelib.hpp>

#include <utl/app.hpp>  // utl::app::wait_key()

#include <thread>       // std::thread
#include <iostream>     // std::cout
#include <exception>    // std::exception

namespace eye { namespace test {
//---------------------------------------------------------------------------
/// @addtogroup eyelib_test_tracker
/// @{

/// Window test type.
enum class Window
{
  gaze,     ///< %Gaze window
  target,   ///< %Target window
};

//-----------------------------------------------------------

/// Test eye gaze and target windows.
inline void
window(unsigned screen_index, Window const& window_type)
{
  std::cout <<'\n'<< "eyelib: Test window" <<'\n';

  auto scr = eye::screen(screen_index);
  std::cout << eye::screen_list(screen_index) <<'\n';

  try
  {
    eye::Tracker tracker("127.0.0.1", "6555", scr);

    switch (window_type)
    {
      case Window::gaze:
      {
        std::cout << "gaze window" <<'\n'<< eye::test::line <<'\n';
        tracker.window();
        std::cout << eye::test::line <<'\n'<< "exit" << std::endl;
        return;
      }
      case Window::target:
      default:
        break;
    }
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
    std::cout << "target window" <<'\n'<< eye::test::line <<'\n';
    tracker.window(points, {250, 1000, 250});
    std::cout << eye::test::line <<'\n'<< "exit" << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

/// @}
//---------------------------------------------------------------------------
} } // eye::test

#endif // EYELIB_TEST_WINDOW_HPP
//===========================================================================//
