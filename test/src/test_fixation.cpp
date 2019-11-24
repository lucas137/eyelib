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

#include "test_fixation.hpp"
#include "eyelib_test.hpp"

#include <eyelib.hpp>

#include <utl/chrono.hpp>       // utl::chrono::datetime
                                // utl::chrono::now_milliseconds
#include <utl/file.hpp>         // utl::file::csv_writer

#include <chrono>       // std::chrono::steady_clock
#include <cmath>        // std::sqrt
#include <exception>    // std::exception
#include <iostream>     // std::cout
#include <string>

namespace {   //-------------------------------------------------------------

// Target duration
constexpr unsigned before_ms = 750;   // milliseconds before active state
constexpr unsigned active_ms = 750;   // milliseconds of active state
constexpr unsigned after_ms  = 0;     // milliseconds after active state

int
fixation_error(eye::Target const& target, eye::PointXY<float> const& gaze)
{
  // Round gaze point coordinates and cast to integers
//  auto gaze_x = static_cast<int>(gaze.x + 0.5);
//  auto gaze_y = static_cast<int>(gaze.y + 0.5);
//
//  auto err_x = (target.x_px - gaze_x);
//  auto err_y = (target.y_px - gaze_y);

  auto err_x = (target.x_px - gaze.x);
  auto err_y = (target.y_px - gaze.y);

  auto err = std::sqrt((err_x*err_x) + (err_y*err_y));

  return static_cast<int>(err + 0.5);
}


void
write_fixations(utl::file::file_writer& log_file,
                std::vector<eye::Fixation> const& fixations,
                std::string const& fixation_method)
{
  // csv_writer accumulates values in comma separated value (CSV)
  // format and writes all values to data_log_ upon destruction
  utl::file::csv_writer(log_file)
    // First row
    <<'\n'<< ""<<""                     // [A-B]
          << "target"<<""<<""           // [C-E]  target
          << ",,,,,,,,,,"               // [F-P]
          << "fixation on object (" +   // [Q]    fixation metrics
             fixation_method + ")"
    // Second row
    <<'\n'<< ""<<""                     // [A-B]
          << "number,x,y"               // [C-E]  target
          << ",,,,,,,,,,"               // [F-P]
          << "count of fixations"       // [Q]    fixation metrics
          << "sum duration (ms)"        // [R]
          << "average duration (ms)"    // [S]
          << "interval (ms)"            // [T]
    <<'\n';

  unsigned t = 0;
  for (auto const& f : fixations)
  {
    utl::file::csv_writer(log_file)
      << ""<<""                 // [A-B]
      << t++ << f.x() << f.y()  // [C-E]  target
      << ",,,,,,,,,,"           // [F-P]
      << f.count()              // [Q]    count of fixations on target
      << f.duration()           // [R]    sum duration of fixations on target
      <<(f.count() ?            // [S]    avg duration of fixations on target
         std::to_string(f.duration() / f.count()) : "")
      << f.interval()           // [T]    time interval between fixations
      <<'\n';
  }
  utl::file::csv_writer(log_file) << '\n';  // blank line
}


} // anonymous --------------------------------------------------------------

namespace eye { namespace test {

FixationTest::FixationTest(eye::Targets const& targets,
                           std::string const& datetime_basic,
                           std::string const& datetime_extended)
{
  data_log_.open("log/" + datetime_basic + "-fixation.csv");

  // To synchronize data for analysis purposes, record current
  // time since epoch and data timestamp in milliseconds
  auto epoch_ms = utl::chrono::now_milliseconds<std::chrono::system_clock>();
  auto time_ms  = utl::chrono::now_milliseconds<std::chrono::steady_clock>();

  // csv_writer accumulates values in comma separated value (CSV)
  // format and writes all values to data_log_ upon destruction
  utl::file::csv_writer(data_log_)

    // Row 1:  Log file info
    << "eyelib-test-fixation" << datetime_extended

    // Row 2-3:  Time sync and sampling
    <<'\n'<< "timestamp (ms)" << "epoch (ms)"     // Session info
          << "target duration (ms)" << "" << ""
          << "DT points" << "DT dmax" << "VT dmax"
//          << "FrameRate" << "SampleSeconds" << "SampleWindow"
    <<'\n'<< time_ms.count() << epoch_ms.count()
          << before_ms << active_ms << after_ms   // Target duration
          << static_cast<unsigned>(DTN) // Dispersion threshold num of points
          << static_cast<float>(DTD)    // Dispersion threshold max dispersion
          << static_cast<float>(VTD)    // Velocity threshold max displacement
//          << static_cast<unsigned>(FrameRate)
//          << static_cast<unsigned>(SampleSeconds)
//          << static_cast<unsigned>(SampleWindow)
    <<'\n'  // Row 4                [column]
    << "eye tracker"                // [A]    timestamp in milliseconds
    << ""                           // [B]    tracking gaze
    << "target"<<""<<""             // [C-E]  target
    << "raw gaze"<<""               // [F-G]  raw gaze
    << "smoothed gaze"<<""          // [H-I]  smoothed gaze
    << "fixation detection"         // [J]    ET fixation
    << ""<<""                       // [K-L]  DT fixation
    << ""<<""                       // [M-N]  VT fixation
    << "fixation on object (ET)"    // [O-T]  fixation metrics
    <<'\n'  // Row 5                [column]
    << "timestamp (ms)"             // [A]    timestamp in milliseconds
    << "tracking"                   // [B]    tracking gaze
    << "x,y,active"                 // [C-E]  target
    << "x,y"                        // [F-G]  raw gaze
    << "x,y"                        // [H-I]  smoothed gaze
    << "ET"                         // [J]    fixation ET flag
    << "DT,dispersion"              // [K-L]  DT algorithm
    << "VT,displacement"            // [M-N]  VT algorithm
    << "target ID"                  // [O]    fixation metrics
    << "fixation point error"       // [P]      error
    << "count of fixations"         // [Q]      count
    << "sum duration (ms)"          // [R]      sum duration
    << "average duration (ms)"      // [S]      average duration
    << "interval (ms)"              // [T]      interval
    <<'\n';

  for (auto const& t : targets)
  {
    et_fixations_.push_back(56);
    et_fixations_.back().position(t.x_px, t.y_px);

    dt_fixations_.push_back(56);
    dt_fixations_.back().position(t.x_px, t.y_px);

    vt_fixations_.push_back(56);
    vt_fixations_.back().position(t.x_px, t.y_px);
  }
}

FixationTest::~FixationTest()
{
  write_fixations(data_log_, et_fixations_, "ET");
  write_fixations(data_log_, dt_fixations_, "DT");
  write_fixations(data_log_, vt_fixations_, "VT");
}

void
FixationTest::on_gaze(eye::Gaze const& gz, bool ts, eye::Target const& tg)
{
  //###############################################################################
  // Count of null gaze point coordinate data?  Use for blink duration threshold?
  //###############################################################################

  bool dt_fixation = false;
  bool vt_fixation = false;

  // Reject gaze point (0, 0) as potential blink
  if ((gz.avg_px.x != 0) && (gz.avg_px.y != 0))
  {
    dt_fixation = dt_.fixation(gz.avg_px.x, gz.avg_px.y);
    vt_fixation = vt_.fixation(gz.avg_px.x, gz.avg_px.y);
  }

  // csv_writer accumulates values in comma separated value (CSV)
  // format and writes all values to data_log_ upon destruction
  utl::file::csv_writer(data_log_)
    << gz.time_ms                           // timestamp in milliseconds (ms)

    <<((gz.tracking.bits == 0x07) ? "true":"false")         // tracking gaze
    << tg.x_px << tg.y_px << (tg.active ? "true":"false")   // target point
    << gz.raw_px.x << gz.raw_px.y                           // raw gaze
    << gz.avg_px.x << gz.avg_px.y                           // smoothed gaze

    <<(gz.fixation ? "true":"false")    // eye tracker fixation flag
    <<(dt_fixation ? "true":"false")    // DT fixation
    << dt_.dispersion()                 // dispersion
    <<(vt_fixation ? "true":"false")    // VT fixation
    << vt_.displacement()               // displacement
    << ""; // Keep log line open

  //###############################################################################
  // Response time until first fixation on target?
  //###############################################################################

  // If target is active
  if (tg.active)
  {
    //for (std::size_t i = 0; i != fixations_.size(); ++i)
    std::size_t i = 0;
    for (auto& f : et_fixations_)
    {
      f.gaze(gz.time_ms, gz.avg_px, gz.fixation);

      // If target is fixated
      if (f.is_fixated())
      {
        utl::file::csv_writer(data_log_)
          //<<(tg.active ? std::to_string(target_error) : "")
          << i                                // Target ID
          << fixation_error(tg, gz.avg_px)    // Target error
          << f.count()      // Count of fixations on target object
          << f.duration()   // Sum total duration of fixations on target object
          <<(f.count() ? std::to_string(f.duration() / f.count()) : "") // Avg
          << f.interval()   // Time duration between fixations on target
          << ""; // Keep log line open
      }
      ++i;
    }
    for (auto& f : dt_fixations_)
    {
      f.gaze(gz.time_ms, gz.avg_px, dt_fixation);
    }
    for (auto& f : vt_fixations_)
    {
      f.gaze(gz.time_ms, gz.avg_px, vt_fixation);
    }
  }

  utl::file::csv_writer(data_log_) << '\n';  // End line
}

//---------------------------------------------------------------------------

void
fixation(unsigned screen_index)
{
  std::cout <<'\n'<< "eyelib: Test fixation algorithms" <<'\n';

  // Log file timestamps
  std::string time_basic    = utl::chrono::datetime_ISO_8601(false);
  std::string time_extended = utl::chrono::datetime_ISO_8601(true);

  // Calibration log file
  utl::file::file_writer calib_log{};
  calib_log.open("log/" + time_basic + "-fix-calib.csv");
  utl::file::csv_writer(calib_log) << "eyelib-test-fixation"
                                   << time_extended <<'\n'<<'\n';

  // Get screen parameters and output to console
  auto scr = eye::screen(screen_index);
  std::cout << eye::screen_list(screen_index) << '\n';

  // 3x3 array of gaze targets
  auto targets  = eye::target_array(scr, 3, 3);

  // Create object to process gaze data
  eye::test::FixationTest fixation_test(targets, time_basic, time_extended);

  //-----------------------------------------------------------
  try
  {
    eye::Tracker tracker("127.0.0.1", "6555", scr);

    // Register lambda to handle streaming gaze data
    tracker.register_handler([&tracker, &fixation_test](eye::Gaze const& gz) {
        eye::Target tg{0, 0, false};
        bool target_sequence = tracker.target(tg);
        fixation_test.on_gaze(gz, target_sequence, tg);
      });

    // Register lambda to handle calibration results
    tracker.register_handler([&calib_log](eye::Calibration const& c){
        // Pretty print to console, and write CSV format to log file
        std::cout << '\n'<< "calibration:" <<'\n'<< c;
        utl::file::csv_writer(calib_log) << eye::csv(c) <<'\n'<<'\n';
      });

    std::cout << "start..." <<'\n'<< eye::test::line <<'\n';

    // Blocks until the window is closed
    tracker.window(targets, {before_ms, active_ms, after_ms});

    std::cout << eye::test::line <<'\n'<< "exit" << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  //-----------------------------------------------------------
}

} } // eye::test
//===========================================================================//
