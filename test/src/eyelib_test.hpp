//===========================================================================//
/// @file
/// @brief    Test gaze handler.
/// @author   Nathan Lucas
/// @date     2015-2016
//===========================================================================//
#ifndef EYELIB_TEST_HPP
#define EYELIB_TEST_HPP

namespace eye {

/**
  @namespace  test
  @brief  Eyelib testing.
*/
namespace test {

/// @addtogroup eyelib_test
/// @{

/// Output separator line.
constexpr auto line = "----------------------------------------------------";

//  #if 1
//    std::string host("127.0.0.1");
//    std::string port("6555");
//    eye::Tracker tracker(host, port, scr);
//  #elif 1
//    eye::Tracker tracker("127.0.0.1", "6555", eye::screen(screen_index));
//  #else
//    eye::Tracker tracker("127.0.0.1", "6555", eye::screen()); // index = 0
//  #endif

/// @}

} } // eye::test

#endif // EYELIB_TEST_HPP
//===========================================================================//
