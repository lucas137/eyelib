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

#include "datalog.hpp"

#include <eyelib.hpp>   // eye::csv, eye::csv_header, eye::Gaze
                        // eye::screen, eye::screen_list, eye::Tracker

#include <utl/app.hpp>      // utl::app::key_wait
#include <utl/chrono.hpp>   // utl::chrono::datetime
                            // utl::chrono::now_milliseconds
#include <utl/file.hpp>     // utl::file::csv_writer
                            // utl::file::remove_extension

#include <cstdlib>    // EXIT_SUCCESS, EXIT_FAILURE
#include <chrono>     // std::chrono::steady_clock
#include <exception>  // std::exception
#include <thread>     // std::thread
#include <iostream>   // std::cout
#include <sstream>    // std::ostringstream
#include <string>     // std::string
#include <vector>     // std::vector

namespace {   //-------------------------------------------------------------

// Output separator line
constexpr auto line = "----------------------------------------------------";

// Default screen index
constexpr unsigned default_screen = 0;

// Output application usage to console
void
print_usage(std::string const& name)
{
  std::cout
    <<"\n  " << name << "  -s:N" <<'\n'
    <<"\n    -s:N  screen number (default: "<< default_screen   <<")"
    <<'\n'
    <<'\n'<< "Writes streaming eye gaze data to file:"
    <<'\n'
    <<'\n'<< "  log/YYYYMMDDThhmmss-datalog.csv"
    <<'\n'
    <<'\n'<< "where 'YYYYMMDD' is the current date and 'hhmmss' is the current time."
    << std::endl;
}

// Argument option
struct Option
{
  std::string key;
  unsigned    val;
  unsigned    min;
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
          err << "  option " << opt.key << " invalid value ("
              << opt.val << " < " << opt.min << ")";
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

} // anonymous --------------------------------------------------------------


namespace eye {


DataLog::DataLog(std::string const& app_name)
: log_file_()
{
  std::string datetime_basic    = utl::chrono::datetime_ISO_8601(false);
  std::string datetime_extended = utl::chrono::datetime_ISO_8601(true);

  log_file_.open("log/" + datetime_basic + "-datalog.csv");

  // csv_writer accumulates values in comma separated value (CSV)
  // format and writes all values to log_file_ upon destruction
  utl::file::csv_writer(log_file_) << app_name << datetime_extended << "\n";
}


int
DataLog::run(unsigned scr)
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

    // Register a lambda expression which will process the first gaze
    // data frame, then replace itself by registering another lambda
    // expression which will invoke the gaze data callback
    tracker.register_handler([this, &tracker](eye::Gaze const& g)
      {
        // First gaze data frame ------------------------------------

        // To synchronize data for analysis purposes, record current
        // time since epoch and gaze data timestamp in milliseconds;
        // Also write the gaze data header to the log file
        auto epoch_ms =
            utl::chrono::now_milliseconds<std::chrono::system_clock>();
        utl::file::csv_writer(log_file_)
            << "epoch_ms"       << "time_ms" <<'\n'   // time sync header
            << epoch_ms.count() << g.time_ms <<'\n'   // time sync data
            << eye::csv_header<eye::Gaze>()  <<'\n';  // gaze data header

        // Subsequent gaze data -------------------------------------

        // Register a lambda expression to invoke gaze data callback
        tracker.register_handler([this](eye::Gaze const& g) { write(g); });

        // ----------------------------------------------------------
      });

    // Run and wait for escape key
    std::cout << "start...\n" << line << "\npress Esc key to stop\n";

    // Start receiving gaze data
    tracker.start();

    // Wait for Escape key (27) in 200 millisecond loop
    std::thread wait_thread([]{ utl::app::key_wait(27, 200); });
    wait_thread.join();       // Block until thread finishes

    std::cout << line << "\nexit\n";
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_FAILURE;
}


// private ------------------------------------------------------------------

// Callback to record gaze data
void
DataLog::write(eye::Gaze const& g)
{
  // csv_writer accumulates values in comma separated value (CSV)
  // format and writes all values to log_file_ upon destruction
  utl::file::csv_writer(log_file_) << eye::csv(g) <<'\n';
}

} // eye

//===========================================================================//

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

  std::cout << "Eye tracker data logger\n";

  Option scr{ "-s", default_screen, 0 };

  if (args.size() > 1)
  {
    for (std::size_t i = 1; i != args.size(); ++i)
    {
      if (!args[i].empty())
      {
        try
        {
          if(!parse(args[i], scr))
          {
            print_usage(args[0]);
            return EXIT_SUCCESS;
          }
        }
        catch (std::exception& e)
        {
          std::cerr << "ERROR: " << e.what() << std::endl;
          print_usage(args[0]);
          return EXIT_FAILURE;
        }
      }
    }
  }

  // Instantiate gaze data logger
  eye::DataLog data_log(args[0]);

  // Run and return
  return data_log.run(scr.val);
}


//===========================================================================//
