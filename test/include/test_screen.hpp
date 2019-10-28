//===========================================================================//
/// @file
/// @brief    Test eye tracker screen.
/// @author   Nathan Lucas
/// @date     2015-2016
//===========================================================================//
#ifndef EYELIB_TEST_SCREEN_HPP
#define EYELIB_TEST_SCREEN_HPP

#include <eyelib/screen.hpp>

namespace eye { namespace test {
//---------------------------------------------------------------------------
/// @addtogroup eyelib_test
/// @{

/// %Screen test type.
enum class Screen
{
  color,              ///< Test eye::ColorRGB
  screen,             ///< Test eye::Screen
  target,             ///< Test eye::Target
  target_container,   ///< Test eye::Targets
  target_duration,    ///< Test eye::TargetDuration
};

/// Test eye tracker gaze data handlers.
void
screen(unsigned screen_index, Screen const& screen_test);

/// @}
//---------------------------------------------------------------------------
} } // eye::test

#endif // EYELIB_TEST_SCREEN_HPP
//===========================================================================//
