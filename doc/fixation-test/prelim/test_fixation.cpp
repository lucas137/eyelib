//===========================================================================//
//  Nathan Lucas
//  2016
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

} // anonymous --------------------------------------------------------------

namespace eye { namespace test {

FixationTest::FixationTest(eye::Targets const& targets)
{
  std::string datetime_basic    = utl::chrono::datetime_ISO_8601(false);
  std::string datetime_extended = utl::chrono::datetime_ISO_8601(true);

  log_file_.open("log/" + datetime_basic + "-fixation.csv");

  // To synchronize data for analysis purposes, record current
  // time since epoch and data timestamp in milliseconds
  auto epoch_ms = utl::chrono::now_milliseconds<std::chrono::system_clock>();
  auto time_ms  = utl::chrono::now_milliseconds<std::chrono::steady_clock>();

  // csv_writer accumulates values in comma separated value (CSV)
  // format and writes all values to log_file_ upon destruction
  utl::file::csv_writer(log_file_)

    // Row 1:  Log file info
    << "eyelib-test-fixation" << datetime_extended

    // Row 2-3:  Time sync and sampling
    <<'\n'<< "time_ms"   << "epoch_ms"              // header
          << "DT_points" << "DT_dmax" << "VT_dmax"
//          << "FrameRate" << "SampleSeconds" << "SampleWindow"
    <<'\n'<< time_ms.count() << epoch_ms.count()    // data
          << static_cast<unsigned>(DTN) // Dispersion threshold num of points
          << static_cast<float>(DTD)    // Dispersion threshold max dispersion
          << static_cast<float>(VTD)    // Velocity threshold max displacement
//          << static_cast<unsigned>(FrameRate)
//          << static_cast<unsigned>(SampleSeconds)
//          << static_cast<unsigned>(SampleWindow)
    <<'\n'  // Row 4                [column]
    << ""                           // [A]    timestamp in milliseconds
    << ""                           // [B]    tracking gaze
    << "Target"<<""<<""<<""         // [C-F]  target
    << "Fixation detection"         // [G]    fixation
    << ""<<""                       // [H-I]    raw gaze
    << ""<<""                       // [J-K]      DT algorithm
    << ""<<""                       // [L-M]      VT algorithm
    << ""<<""                       // [N-O]    smoothed gaze
    << ""<<""                       // [P-Q]      DT algorithm
    << ""<<""                       // [R-S]      VT algorithm
    << "Fixation metrics using ET"  // [P]    fixation error
    << ""                           // [Q]      count
    << ""<<""                       // [R-S]    duration
    << ""                           // [T]      interval
    <<'\n'  // Row 5                [column]
    << ""                           // [A]    timestamp in milliseconds
    << ""                           // [B]    tracking gaze
    << "sequence,point"<<""<<""     // [C-F]  target
    << ""                           // [G]    fixation
    << "raw gaze"<<""               // [H-I]    raw gaze
    << ""<<""                       // [J-K]      DT algorithm
    << ""<<""                       // [L-M]      VT algorithm
    << "smoothed gaze"<<""          // [N-O]    smoothed gaze
    << ""<<""                       // [P-Q]      DT algorithm
    << ""<<""                       // [R-S]      VT algorithm
    << "target"                     // [T]    fixation error
    << ""                           // [U]      count
    << "duration (ms)"<<""          // [V-W]    duration
    << ""                           // [X]      interval
    <<'\n'  // Row 6                [column]
    << "time_ms"                    // [A]    timestamp in milliseconds
    << "tracking"                   // [B]    tracking gaze
    << "started,x,y,active"         // [C-F]  target
    << "ET"                         // [G]    fixation ET flag
    << "x,y"                        // [H-I]    raw gaze
    << "DT fixation,dispersion"     // [J-K]      DT algorithm
    << "VT fixation,displacement"   // [L-M]      VT algorithm
    << "x,y"                        // [N-O]    smoothed gaze
    << "DT fixation,dispersion"     // [P-Q]      DT algorithm
    << "VT fixation,displacement"   // [R-S]      VT algorithm
    << "error"                      // [T]    fixation error
    << "count"                      // [U]      count
    << "sum,avg"                    // [V-W]    duration
    << "interval"                   // [X]      interval
    <<'\n';

  for (auto const& t : targets)
  {
    fixations_.push_back(56);
    fixations_.back().position(t.x_px, t.y_px);
  }
}

FixationTest::~FixationTest()
{
  // csv_writer accumulates values in comma separated value (CSV)
  // format and writes all values to log_file_ upon destruction
  utl::file::csv_writer(log_file_)
    // First row
    <<'\n'<< ""<<""             // [A-B]
          << "Target,,"         // [C-E]  target
          << ",,,,,,,,,,,,,,"   // [F-T]
          << "Fixation"         // [U]    fixation count
          << "duration (ms),"   // [V-W]    duration
          << ""                 // [X]      interval
    // Second row
    <<'\n'<< ""<<""             // [A-B]
          << "number,x,y"       // [C-E]  target
          << ",,,,,,,,,,,,,,"   // [F-T]
          << "count"            // [U]    fixation count
          << "sum,avg"          // [V-W]    duration
          << "interval"         // [X]      interval
    <<'\n';

  unsigned t = 0;
  for (auto const& f : fixations_)
  {
    utl::file::csv_writer(log_file_)
      << ""<<""                 // [A-B]
      << t++ << f.x() << f.y()  // [C-E]  target
      << ",,,,,,,,,,,,,,"       // [F-T]
      << f.count()              // [U]    count of fixations on target
      << f.duration()           // [V]    sum duration of fixations on target
      <<(f.count() ?            // [W]    avg duration of fixations on target
         std::to_string(f.duration() / f.count()) : "")
      << f.interval()           // [X]    time interval between fixations
      <<'\n';
  }
}

void
FixationTest::on_gaze(eye::Gaze const& gz, bool ts, eye::Target const& tg)
{
  // csv_writer accumulates values in comma separated value (CSV)
  // format and writes all values to log_file_ upon destruction
  utl::file::csv_writer(log_file_)
    << gz.time_ms                           // timestamp in milliseconds (ms)

    <<((gz.tracking.bits == 0x07) ? "true":"false")         // tracking gaze

    <<(ts ? "true":"false")                                 // target sequence
    << tg.x_px << tg.y_px << (tg.active ? "true":"false")   // target point

    <<(gz.fixation ? "true":"false")                        // fixation flag

    << gz.raw_px.x << gz.raw_px.y                           // raw gaze
    <<(raw_dt_.fixation(gz.raw_px.x, gz.raw_px.y) ? "true":"false") // DT fix
    << raw_dt_.dispersion()                                   // dispersion
    <<(raw_vt_.fixation(gz.raw_px.x, gz.raw_px.y) ? "true":"false") // VT fix
    << raw_vt_.displacement()                                 // displacement

    << gz.avg_px.x << gz.avg_px.y                           // smoothed gaze
    <<(avg_dt_.fixation(gz.avg_px.x, gz.avg_px.y) ? "true":"false") // DT fix
    << avg_dt_.dispersion()                                   // dispersion
    <<(avg_vt_.fixation(gz.avg_px.x, gz.avg_px.y) ? "true":"false") // VT fix
    << avg_vt_.displacement()                                 // displacement
    << ""   // ??
    ; // End log input

  //###############################################################################
  // Run randomized targets multiple times during same test

  // Response time until first fixation on target?

  // Derive DT and VT parameters offline?
  // Compute dispersion
  //###############################################################################

  //if (tg.active) {}

  // If target is active
  if (tg.active)
  {
    //for (std::size_t i = 0; i != fixations_.size(); ++i)
    for (auto& f : fixations_)
    {
      f.gaze(gz.time_ms, gz.avg_px, gz.fixation);

      // If target is fixated
      if (f.is_fixated())
      {
        utl::file::csv_writer(log_file_)
          //<<(tg.active ? std::to_string(target_error) : "")
          << fixation_error(tg, gz.avg_px)    // Target error

          << f.count()      // Count of fixations on target object
          << f.duration()   // Sum total duration of fixations on target object
          <<(f.count() ? std::to_string(f.duration() / f.count()) : "") // Avg
          << f.interval()   // Time duration between fixations on target
          ; // End log input
      }
    }
  }

  utl::file::csv_writer(log_file_) << '\n';  // End line
}

//---------------------------------------------------------------------------

void
fixation(unsigned screen_index)
{
  std::cout <<'\n'<< "eyelib: Test fixation algorithms" <<'\n';

  auto scr = eye::screen(screen_index);
  std::cout << eye::screen_list(screen_index) << '\n';

  auto targets = eye::target_array(scr, 3, 3);
  eye::test::FixationTest fixation_test(targets);

  try
  {
    eye::Tracker tracker("127.0.0.1", "6555", scr);

    tracker.register_handler([&tracker, &fixation_test](eye::Gaze const& gz) {
        eye::Target tg{0, 0, false};
        bool target_sequence = tracker.target(tg);
        fixation_test.on_gaze(gz, target_sequence, tg);
      });

    tracker.window(targets, {500, 1000, 250});
    std::cout << eye::test::line <<'\n'<< "exit" << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

} } // eye::test
//===========================================================================//
