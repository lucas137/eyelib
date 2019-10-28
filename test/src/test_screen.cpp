//===========================================================================//
//  Nathan Lucas
//  2016
//===========================================================================//

#include "test_screen.hpp"

#include <eyelib.hpp>

#include <iostream>   // std::cout
#include <exception>  // std::exception

namespace eye { namespace test {

void
screen(unsigned screen_index, Screen const& screen_test)
{
  auto scr = eye::screen(screen_index);
  int cx = scr.w_px/2;    // center of screen
  int cy = scr.h_px/2;

  std::cout <<'\n'<< "eyelib: Test screen" <<'\n';
  try
  {
    switch(screen_test)
    {
      case Screen::color:
      {
        ColorRGB color = {128,192,255};
        std::cout << "----------------------------------------------------"
            <<'\n'<< "ColorRGB"
            <<'\n'<< "red   : " << static_cast<unsigned>(color.r)
            <<'\n'<< "green : " << static_cast<unsigned>(color.g)
            <<'\n'<< "blue  : " << static_cast<unsigned>(color.b) <<'\n';
        break;
      }
      case Screen::screen:
      {
        std::cout << "----------------------------------------------------"
            <<'\n'<< "Screen" << '\n'
            <<'\n'<< "default:"
            <<'\n'<< eye::Screen()  << '\n'
            <<'\n'<< "index=" << screen_index << ":"
            <<'\n'<< scr  << '\n'
            <<'\n'<< "eye::screen_list(" << screen_index << "):"
            <<'\n'<< eye::screen_list(screen_index) << '\n';
        break;
      }
      case Screen::target:
      {
        std::cout << "----------------------------------------------------"
            <<'\n'<< "Target:"
            <<'\n'<< eye::Target{cx, cy, true} <<'\n';
        break;
      }
      case Screen::target_container:
      {
        std::cout << "----------------------------------------------------"
            <<'\n'<< "Targets:" << '\n';
        eye::Targets targets {              // "manually" define targets
            { cx - 100, cy - 100, true },
            { cx + 100, cy - 100, false },
            { cx - 100, cy + 100, true },
            { cx + 100, cy + 100, false }
          };
        for (auto const& t : targets)
        {
          std::cout << t << '\n';
        }
        std::cout <<'\n'<< "Uniform distribution on screen:" <<'\n';
        auto tu = eye::target_array(scr, 3, 3);
        for (auto const& t : tu)
        {
          std::cout << t << '\n';
        }
        std::cout <<'\n'<< "Uniform distribution with defined margins:"<<'\n';
        auto tm = eye::target_array(scr, 3, 3, 120, 120);
        for (auto const& t : tm)
        {
          std::cout << t << '\n';
        }
        break;
      }
      case Screen::target_duration:
      {
        TargetDuration duration{150,250,100};
        std::cout << "----------------------------------------------------"
            <<'\n'<< "TargetDuration"
            <<'\n'<< "before_ms : " << duration.before_ms
            <<'\n'<< "active_ms : " << duration.active_ms
            <<'\n'<< "after_ms  : " << duration.after_ms <<'\n';
        break;
      }
      default:
        break;  // invalid
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  std::cout << "----------------------------------------------------\n";
}

} } // eye::test
//===========================================================================//
