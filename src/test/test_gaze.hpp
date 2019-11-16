//===========================================================================//
/// @file
/// @brief    Test gaze handler.
/// @author   Nathan Lucas
/// @date     2015-2016
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
