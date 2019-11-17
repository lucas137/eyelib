//===========================================================================//
//  Nathan Lucas
//  2016
//===========================================================================//

#include "test_message.hpp"

#include <eyelib/tracker.hpp>   // eye::tracker::message::debug

namespace eye { namespace test {

void
message(TestMessage const& t)
{
  tracker::message::debug::message_test(t);
}

} } // eye::test
//===========================================================================//
