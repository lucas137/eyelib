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
/// @brief    Test gaze handler.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYELIB_TEST_GAZE_HPP
#define EYELIB_TEST_GAZE_HPP

namespace eye { namespace test {
//---------------------------------------------------------------------------
/// @addtogroup eyelib_test
/// @{

/// Handler type.
enum class Handler
{
  function,   ///< Free function.
  lambda,     ///< Lambda expression.
  member,     ///< Class member function.
};

/// Test eye tracker gaze data handlers.
void
gaze_handler(unsigned screen_index, Handler const& handler_type);

/// @}
//---------------------------------------------------------------------------
} } // eye::test

#endif // EYELIB_TEST_GAZE_HPP
//===========================================================================//
