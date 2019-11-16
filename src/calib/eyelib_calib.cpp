//===========================================================================//
/// @file
/// @brief    Eye tracker calibrator.
/// @author   Nathan Lucas
/// @date     2015-2016
//===========================================================================//

#include <eyelib.hpp>

#include <exception>  // std::exception
#include <iostream>   // std::cout
#include <sstream>    // std::ostringstream
#include <string>     // std::string
#include <vector>     // std::vector

namespace {   //-------------------------------------------------------------

// Output separator line
constexpr auto line = "----------------------------------------------------";

constexpr unsigned default_screen   = 0;
constexpr unsigned default_x_points = 4;
constexpr unsigned default_y_points = 3;

constexpr unsigned minimum_x_points = 3;
constexpr unsigned minimum_y_points = 3;

// Default target time (millisecond)
constexpr unsigned default_t_before =  500;   // delay before active state
constexpr unsigned default_t_active = 1000;   // active state duration
constexpr unsigned default_t_after  =  500;   // delay before next target

// Default background color
constexpr unsigned default_bg_r     = 149;    // red
constexpr unsigned default_bg_g     = 149;    // green
constexpr unsigned default_bg_b     = 149;    // blue

// Output application usage to console
void
print_usage(std::string const& name)
{
  std::cout
    <<'\n'<< "Opens a window to perform eye tracker calibration using an"
    <<'\n'<< "array of fixation targets displayed in randomized order."
    <<'\n'
    <<"\n  " << name << "  [-s:N] [-x:N -y:N] [-t0:N] [-t1:N] [-t2:N]"
    <<'\n'
    <<"\n    -s:N    screen number (default: "<< default_screen   <<")"
    <<'\n'
    <<"\n    target point array:"
    <<'\n'
    <<"\n    -x:N    number of targets horizontally (default: "<< default_x_points <<")"
    <<"\n    -y:N    number of targets vertically   (default: "<< default_y_points <<")"
    <<'\n'
    <<"\n    target time (milliseconds):"
    <<'\n'
    <<"\n    -t0:N   pre-active delay  (default: "<< default_t_before <<")"
    <<"\n    -t1:N   active duration   (default: "<< default_t_active <<")"
    <<"\n    -t2:N   post-active delay (default: "<< default_t_after  <<")"
    <<'\n'
    <<"\n    background color:"
    <<'\n'
    <<"\n    -r:N    red   (default: "<< default_bg_r <<")"
    <<"\n    -g:N    green (default: "<< default_bg_g <<")"
    <<"\n    -b:N    blue  (default: "<< default_bg_b <<")"
    <<'\n'
    <<'\n'<< "Specify -x and/or -y to change the number of fixation targets for calibration."
    <<'\n'<< "The outer-most targets are offset from screen edge by a margin equal to 10%"
    <<'\n'<< "of the minimum screen dimension.  Upon the user initiating calibration, each"
    <<'\n'<< "target is diplayed once in random order.  Each target has three phases:"
    <<'\n'
    <<'\n'<< "  pre-active  -- solid gray circle"
    <<'\n'<< "  active      -- solid gray cirlce with black dot in the center"
    <<'\n'<< "  post-active -- solid gray circle"
    <<'\n'
    <<'\n'<< "Calibration data samples are collected during the 'active' phase."
    <<'\n'<< "Specify -t0, -t1, and/or -t2 to set the respective phase duration(s)."
    << std::endl;
}

// Argument option
struct Option
{
  std::string key;
  unsigned    val;
  unsigned    min;
  unsigned    max;
};

// Check argument `arg` for key matching option `opt`, and if
// the key is found get and save the option value to `opt.val`
bool
parse(std::string const& arg, Option& opt)
{
  try
  {
    auto d = arg.find(":");
    if (d != std::string::npos)
    {
      if (arg.substr(0, d) == opt.key)
      {
        auto str = arg.substr(d + 1);
        opt.val = std::stoul(str);
        if (opt.val < opt.min)
        {
          std::ostringstream err;
          err << "option " << opt.key << " invalid value ("
              << opt.val << " < " << opt.min << ")";
          throw std::runtime_error(err.str());
        }
        if ((opt.max > 0) && (opt.val > opt.max))
        {
          std::ostringstream err;
          err << "option " << opt.key << " invalid value ("
              << opt.val << " > " << opt.max << ")";
          throw std::runtime_error(err.str());
        }
        return true;
      }
    }
  }
  catch (std::exception& e)
  {
    std::ostringstream err;
    err << "exception thrown parsing " << arg << '\n' << e.what();
    throw std::runtime_error(err.str());
  }
  return false;
}

//-----------------------------------------------------------

// Eye tracker calibration
void
calibrate(unsigned scr, unsigned ptx, unsigned pty,
          eye::TargetDuration const& duration,
          eye::ColorRGB const& background)
{
  // Create a screen object that contains
  // parameters for the specified display
  auto scrn = eye::screen(scr);

  // Output to the console a list of each screen and its resolution and
  // position, and indicate which screen was specified for eye tracking
  std::cout <<'\n'<< eye::screen_list(scr) <<'\n';
  try
  {
    // Create eye tracker object and connect to
    // an eye tracker server on the local host
    eye::Tracker tracker("127.0.0.1", "6555", scrn);

    // Register a handler that will be called whenever calibration
    // results are received from the eye tracker server
    tracker.register_handler(
        // Lambda expression
        [](eye::Calibration const& cal)
        {
          // Output calibration results to console
          std::cout << "calibration result:" <<'\n'<< cal <<'\n';
        }
      );

    // Output the number of points, horizontal and
    // vertical, in the calibration point array,
    std::cout <<'\n'<< "calibration array: " << ptx <<'x'<< pty <<'\n';

    // Uniformly distributed targets
    auto points = eye::target_array(scrn, ptx, pty);

    std::cout << "calibrator start..." <<'\n'<< line <<'\n';

    // Open a calibration window allowing the user to calibrate tracker.
    // The window calls tracker.start() to start the tracker if it has not
    // already been started.  calibrate() blocks until the window is closed.
    tracker.calibrate(points, duration, background);

    std::cout << line <<'\n'<< "calibrator exit" <<'\n';
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

} // anonymous --------------------------------------------------------------

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

  std::cout << "Eye tracker calibrator\n";

  Option scr{ "-s", default_screen, 0, 0 };

  Option ptx{ "-x", default_x_points, minimum_x_points, 0 };
  Option pty{ "-y", default_y_points, minimum_y_points, 0 };

  Option tt0{ "-t0", default_t_before, 0, 0 };
  Option tt1{ "-t1", default_t_active, 0, 0 };
  Option tt2{ "-t2", default_t_after, 0, 0 };

  Option bgr{ "-r", default_bg_r, 0, 255 };
  Option bgg{ "-g", default_bg_g, 0, 255 };
  Option bgb{ "-b", default_bg_b, 0, 255 };

  if (args.size() > 1)
  {
    for (std::size_t i = 1; i != args.size(); ++i)
    {
      if (!args[i].empty())
      {
        try
        {
          if(!parse(args[i], scr) &&
             !parse(args[i], ptx) &&
             !parse(args[i], pty) &&
             !parse(args[i], tt0) &&
             !parse(args[i], tt1) &&
             !parse(args[i], tt2) &&
             !parse(args[i], bgr) &&
             !parse(args[i], bgg) &&
             !parse(args[i], bgb))
          {
            print_usage(args[0]);
            return EXIT_SUCCESS;
          }
        }
        catch (std::exception& e)
        {
          std::cerr << "\nERROR: " << e.what()
                    << "\ntype '" << args[0] << " -h' for help"<< std::endl;
          return EXIT_FAILURE;
        }
      }
    }
  }

  calibrate(scr.val, ptx.val, pty.val,
            { tt0.val, tt1.val, tt2.val },
            { static_cast<unsigned char>(bgr.val),
              static_cast<unsigned char>(bgg.val),
              static_cast<unsigned char>(bgb.val) });

  return EXIT_SUCCESS;
}

//===========================================================================//
