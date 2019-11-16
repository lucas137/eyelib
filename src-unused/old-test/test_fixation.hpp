//===========================================================================//
/// @file
/// @brief    Test fixation detection algorithms.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYELIB_TEST_FIXATION_HPP
#define EYELIB_TEST_FIXATION_HPP

#include "eyelib_test.hpp"

#include <iostream>   // std::cout
#include <exception>  // std::exception

namespace eye { namespace test {
//---------------------------------------------------------------------------
/// @addtogroup eyelib_test
/// @{

/// Test eye tracker gaze data processing algorithms.
inline void
fixation(unsigned screen_index)
{
  std::cout <<'\n'<< "eyelib: Test eye gaze metrics" <<'\n';

  auto scr = eye::screen(screen_index);
  std::cout << eye::screen_list(screen_index) << '\n';

  eye::DispersionThreshold  dt(10, 72.0);
  eye::VelocityThreshold    vt(10.0);

  try
  {
    eye::Tracker tracker("127.0.0.1", "6555", scr);

    std::cout << "dispersion threshold (dt) and velocity threshold (vt)"
              <<'\n'<< eye::test::line <<'\n'
              << "time_ms,eyes,fixation,raw_x,raw_y,dt,vt,"
                                       "avg_x,avg_y,dt,vt" << '\n';

    tracker.register_handler([&dt, &vt](eye::Gaze const& g)
      {
        std::cout << g.time_ms
            <<',' << (g.tracking.eyes ? "true":"false")   // potential blink
            <<',' << (g.fixation ? "true":"false")
            <<',' << g.raw_px.x <<','<< g.raw_px.y
            <<',' << (dt.fixation(g.raw_px.x, g.raw_px.y) ? "true":"false")
            <<',' << (vt.fixation(g.raw_px.x, g.raw_px.y) ? "true":"false")
            <<',' << g.avg_px.x <<','<< g.avg_px.y
            <<',' << (dt.fixation(g.avg_px.x, g.avg_px.y) ? "true":"false")
            <<',' << (vt.fixation(g.avg_px.x, g.avg_px.y) ? "true":"false")
            <<'\n';
      });

    auto points = eye::target_array(scr, 3, 3);
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

#endif // EYELIB_TEST_FIXATION_HPP
//===========================================================================//
