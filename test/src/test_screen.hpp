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
/// @brief    Test eye tracker screen.
/// @author   Nathan Lucas
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
