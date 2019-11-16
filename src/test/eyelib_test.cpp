//===========================================================================//
//  Nathan Lucas
//  2016
//===========================================================================//

#include "test_fixation.hpp"    // eye::test::fixation
#include "test_gaze.hpp"        // eye::test::gaze_handler
#include "test_message.hpp"     // eye::test::message
#include "test_metrics.hpp"     // eye::test::metrics
#include "test_screen.hpp"      // eye::test::screen
#include "test_tracker.hpp"     // eye::test::tracker

#include <eyelib.hpp>   // eye::tracker::message::debug::TestMessage

#include <string>     // std::string
#include <vector>     // std::vector
#include <iostream>   // std::cout

namespace {   //-------------------------------------------------------------

void
print_usage(std::string const& name)
{
  std::cout
    << "\nEye tracking library test."
    << '\n'
    << "\n  " << name << "  test [-s:N]"
    << '\n'
    << "\n    test:"
    << '\n'
    << "\n      -e    eye gaze metrics"
    << "\n      -f    fixation algorithms"
    << '\n'
    << "\n      -g:f  gaze data function handler"
    << "\n      -g:l  gaze data lambda handler"
    << "\n      -g:m  gaze data member handler"
    << '\n'
    << "\n      -m    messages (all tests)"
    << "\n      -m:c    calibration"
    << "\n      -m:o    ostream string"
    << "\n      -m:p    predefined"
    << "\n      -m:r    requests"
    << '\n'
    << "\n      -s    screen data structure and list"
    << "\n      -s:c    color"
    << "\n      -s:t    target"
    << "\n      -s:tc   target container"
    << "\n      -s:td   target duration"
    << '\n'
    << "\n      -t    tracker"
    << "\n      -x    code snippet"
    << '\n'
    << "\n    option:"
    << '\n'
    << "\n      -s:N  screen number [0-3] (default: 0)"
    << std::endl;
}

struct Test
{
  void test1(int n)
  {
    std::cout << "1:" << n << '\n';
  }

  void test2(int n)
  {
    std::cout << "2:" << n << '\n';
  }
};

} // anonymous --------------------------------------------------------------

namespace eye { namespace test {  //-----------------------------------------

void
code_snippet()
{
  //#define NEAT_OPERATOR_COMBO_TRICK
  #ifdef NEAT_OPERATOR_COMBO_TRICK
  unsigned x = 10;
  while (x --> 0)   // while( (x--) > 0 )
  {
    std::cout << x;
  }
  std::cout << std::endl;
  #endif

}

bool
eyelib_test(std::string const& arg, unsigned scr)
{
  using TestMessage = eye::tracker::message::debug::TestMessage;

       if (arg == "-e")     { metrics(scr); }
  else if (arg == "-f")     { fixation(scr); }

  else if (arg == "-g:f")   { gaze_handler(scr, Handler::function); }
  else if (arg == "-g:l")   { gaze_handler(scr, Handler::lambda); }
  else if (arg == "-g:m")   { gaze_handler(scr, Handler::member); }

  else if (arg == "-m")     { message(TestMessage::all); }
  else if (arg == "-m:c")   { message(TestMessage::calibration); }
  else if (arg == "-m:o")   { message(TestMessage::ostream_string); }
  else if (arg == "-m:p")   { message(TestMessage::predefined); }
  else if (arg == "-m:r")   { message(TestMessage::requests); }

  else if (arg == "-s")     { screen(scr, Screen::screen); }
  else if (arg == "-s:c")   { screen(scr, Screen::color); }
  else if (arg == "-s:t")   { screen(scr, Screen::target); }
  else if (arg == "-s:tc")  { screen(scr, Screen::target_container); }
  else if (arg == "-s:td")  { screen(scr, Screen::target_duration); }

  else if (arg == "-t")     { tracker(scr); }
  else if (arg == "-x")     { code_snippet(); }
  else
  {
    return false;
  }
  return true;
}

} } // eye::test ------------------------------------------------------------

int
main(int argc, char* argv[])
{
  std::vector<std::string> args(argv, argv + argc);   // arguments

  // Parse program file name from path
 #ifdef _WIN32
  std::size_t file = args[0].find_last_of('\\') + 1;
 #else
  std::size_t file = args[0].find_last_of('/') + 1;
 #endif
  std::size_t ext = args[0].find_last_of('.');
  args[0] = args[0].substr(file, ext - file);

  unsigned screen_index = 0;

  // Process optional arguments
  if (args.size() > 1)
  {
    for (std::size_t i = 1; i != args.size(); ++i)
    {
      if (!args[i].empty())
      {
             if (args[i] == "-s:0") { screen_index = 0; }
        else if (args[i] == "-s:1") { screen_index = 1; }
        else if (args[i] == "-s:2") { screen_index = 2; }
        else if (args[i] == "-s:3") { screen_index = 3; }
      }
    }
  }
  // Process test arguments
  if (args.size() > 1)
  {
    for (std::size_t i = 1; i != args.size(); ++i)
    {
      if (!args[i].empty())
      {
        // Ignore screen number arguments (already processed above)
        if (args[i] == "-s:0" || args[i] == "-s:1" ||
            args[i] == "-s:2" || args[i] == "-s:3") {}
        // Check for test argument
        else if (!eye::test::eyelib_test(args[i], screen_index))
        {
          // Invalid argument or request for help (e.g., "-?")
          goto endloop;
        }
      }
    }
    return EXIT_SUCCESS;
  }
endloop:
  // User requested help info or supplied invalid argument(s)
  print_usage(args[0]);
}

//===========================================================================//
