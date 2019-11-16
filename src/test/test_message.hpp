//===========================================================================//
/// @file
/// @brief    Test eye tracker messages.
/// @author   Nathan Lucas
/// @date     2015-2016
//===========================================================================//
#ifndef EYELIB_TEST_MESSAGE_HPP
#define EYELIB_TEST_MESSAGE_HPP

#include <eyelib.hpp>

namespace eye { namespace test {
//---------------------------------------------------------------------------
/// @addtogroup eyelib_test
/// @{

/// Message test type.
using TestMessage = tracker::message::debug::TestMessage;

/// Test eye tracker message serialization/deserialization.
void
message(TestMessage const& t);

/// @}
//---------------------------------------------------------------------------
} } // eye::test

#endif // EYELIB_TEST_MESSAGE_HPP
//===========================================================================//
