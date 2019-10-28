//===========================================================================//
/// @file
/// @brief    Test eye tracker calibration.
/// @author   Nathan Lucas
/// @date     2015-2016
//===========================================================================//
#ifndef EYELIB_TEST_CALIBRATION_HPP
#define EYELIB_TEST_CALIBRATION_HPP

#include <eyelib.hpp>

#include <iostream>   // std::cout
#include <exception>  // std::exception

namespace eye { namespace test {
//---------------------------------------------------------------------------
/// @addtogroup eyelib_test_calib
/// @{

namespace detail {

/// Free function calibration handler.
inline void
handle_calib(eye::Calibration const& cal)
{
  std::cout << cal << std::endl;
}

}

/// Test eye tracker calibration.
inline void
calibration(unsigned screen_index)
{
  std::cout <<'\n'<< "eyelib: Test calibration" <<'\n';

  auto scr = eye::screen(screen_index);
  std::cout << eye::screen_list(screen_index) << '\n';
  try
  {
    eye::Tracker tracker("127.0.0.1", "6555", scr);

    // Register calibration handler
   #if 1
    tracker.register_handler(
        [](eye::Calibration const& cal)
        {
          std::cout << cal << '\n';

          // Check if calibration results are acceptable
        }
      );
   #elif 1
    tracker.register_handler(detail::handle_calib);
   #endif
    // Uniformly distributed targets
    auto points = eye::target_array(scr, 3, 3);

    std::cout << "start..." << '\n' << eye::test::line << '\n';

    // Open a calibration window allowing the user to calibrate tracker.
    // The window calls tracker.start() to start the tracker if it has not
    // already been started.  calibrate() blocks until the window is closed.
    tracker.calibrate(points, {250, 500, 250});

    std::cout << eye::test::line << '\n' << "exit" << '\n';
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

/// @}
//---------------------------------------------------------------------------
} } // eye::test

#endif // EYELIB_TEST_CALIBRATION_HPP
//===========================================================================//
