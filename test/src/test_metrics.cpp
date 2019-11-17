//===========================================================================//
//  Nathan Lucas
//  2016
//===========================================================================//

#include "test_metrics.hpp"
#include "eyelib_test.hpp"

#include <eyelib.hpp>

#include <utl/chrono.hpp>       // utl::chrono::datetime
                                // utl::chrono::now_milliseconds
#include <utl/file.hpp>         // utl::file::csv_writer

#include <chrono>     // std::chrono::steady_clock
#include <iostream>   // std::cout
#include <exception>  // std::exception

namespace {   //-------------------------------------------------------------
} // anonymous --------------------------------------------------------------

namespace eye { namespace test {

GazeMetrics::GazeMetrics()
{
  std::string datetime_basic    = utl::chrono::datetime_ISO_8601(false);
  std::string datetime_extended = utl::chrono::datetime_ISO_8601(true);

  log_file_.open("log/" + datetime_basic + "-metrics.csv");

  // To synchronize data for analysis purposes, record current
  // time since epoch and data timestamp in milliseconds
  auto epoch_ms = utl::chrono::now_milliseconds<std::chrono::system_clock>();
  auto time_ms  = utl::chrono::now_milliseconds<std::chrono::steady_clock>();

  // csv_writer accumulates values in comma separated value (CSV)
  // format and writes all values to log_file_ upon destruction
  utl::file::csv_writer(log_file_)

    // Row 1:  Log file info
    << "eyelib-test" << datetime_extended

    // Row 2-3:  Time sync and sampling
    <<'\n'<< "time_ms"   << "epoch_ms"              // header
          << "DT_points" << "DT_dmax" << "VT_dmax"
          << "FrameRate" << "SampleSeconds" << "SampleWindow"
    <<'\n'<< time_ms.count() << epoch_ms.count()    // data
          << static_cast<unsigned>(DTN) // Dispersion threshold num of points
          << static_cast<float>(DTD)    // Dispersion threshold max dispersion
          << static_cast<float>(VTD)    // Velocity threshold max displacement
          << static_cast<unsigned>(FrameRate)
          << static_cast<unsigned>(SampleSeconds)
          << static_cast<unsigned>(SampleWindow)

    // Row 4-6:  Data header
    // Eye property type header
    <<'\n'<<  ",blink,,,,,,"            // detection, duration, and interval
          <<  "target,,,"               // gaze target point
          <<  "fixation,,,,,,,,,,,,,,"  // detection, duration, and interval
          <<  "saccade,,"               // distance squared
          <<  "pupillometry"            // pupil size
    // Eye property header
    <<'\n'<< ",flag"                        // blink flag
          << "duration,," << "interval,,"   // blink metrics
          << "sequence" << "point,,"        // target
          << "flag" << "detection,,,,,,,"   // fixation detection
          << "duration,," << "interval,,"   // fixation metrics
          << "distance_sq,,"                // saccade
          << "pupil_size," << "metrics"     // pupillometry
    // Data value header
    <<'\n'<< "time_ms"              // timestamp in milliseconds
          << "eyes_closed"          // blink flag (true or false)
          << "mean,max,sum"         // blink duration
          << "mean,max,sum"         // blink interval
          << "started,x,y,active"   // target
          << "fixation"             // fixation flag (true or false)
          << "raw_x,raw_y,dt,vt"    // fixation detection, raw gaze point
          << "avg_x,avg_y,dt,vt"    // fixation detection, smoothed gaze point
          << "mean,max,sum"         // fixation duration
          << "mean,max,sum"         // fixation interval
          << "mean,max,sum"         // saccade distance squared
          << "left_pupil_size"      // pupil size <float>
          << "right_pupil_size"     // pupil size <float>
          << "mean,max,sum"         // pupil size metrics
    <<'\n';
}

void
GazeMetrics::on_gaze(eye::Gaze const& gz, bool ts, eye::Target const& tg)
{
  bool eyes_closed = !gz.tracking.eyes;             // Not tracking both eyes
  //bool eyes_closed = (gz.tracking.bits == 0x04);  // Tracking face or an eye

  blink.update(gz.time_ms, eyes_closed);
  fixation.update(gz.time_ms, gz.fixation);
  saccade.update(gz.avg_px, !gz.fixation);
  pupil.update(gz.pupil_left.size, gz.pupil_right.size);

  // csv_writer accumulates values in comma separated value (CSV)
  // format and writes all values to log_file_ upon destruction
  utl::file::csv_writer(log_file_)
    << gz.time_ms                       // timestamp in milliseconds (ms)

    << (eyes_closed ? "true":"false")   // possible blink
    << blink.duration().mean()          // blink duration (ms)
    << blink.duration().max()
    << blink.duration().sum()
    << blink.interval().mean()          // blink interval (ms)
    << blink.interval().max()
    << blink.interval().sum()

    << (ts ? "true":"false")                                // target sequence
    << tg.x_px << tg.y_px << (tg.active ? "true":"false")   // target point

    <<(gz.fixation ? "true":"false")                           // fixation flag
    << gz.raw_px.x << gz.raw_px.y                              // raw gaze
    <<(dt.fixation(gz.raw_px.x, gz.raw_px.y) ? "true":"false") // DT algorithm
    <<(vt.fixation(gz.raw_px.x, gz.raw_px.y) ? "true":"false") // VT algorithm
    << gz.avg_px.x << gz.avg_px.y                              // smoothed gaze
    <<(dt.fixation(gz.avg_px.x, gz.avg_px.y) ? "true":"false") // DT algorithm
    <<(vt.fixation(gz.avg_px.x, gz.avg_px.y) ? "true":"false") // VT algorithm

    << fixation.duration().mean()       // fixation duration (ms)
    << fixation.duration().max()
    << fixation.duration().sum()
    << fixation.interval().mean()       // fixation interval (ms)
    << fixation.interval().max()
    << fixation.interval().sum()

    << saccade.distance_sq().mean()     // saccade distance squared (px^2)
    << saccade.distance_sq().max()
    << saccade.distance_sq().sum()

    << gz.pupil_left.size               // pupil size <float>
    << gz.pupil_right.size
    << pupil.pupil_size().mean()        // pupil size metrics
    << pupil.pupil_size().max()
    << pupil.pupil_size().sum()
    <<'\n';
}

//----------------------------------------------------------------------------

void
metrics(unsigned screen_index)
{
  std::cout <<'\n'<< "eyelib: Test eye gaze metrics" <<'\n';

  auto scr = eye::screen(screen_index);
  std::cout << eye::screen_list(screen_index) << '\n';

  eye::test::GazeMetrics  metrics;
  try
  {
    eye::Tracker tracker("127.0.0.1", "6555", scr);

    tracker.register_handler([&tracker, &metrics](eye::Gaze const& gz) {
        eye::Target tg{0, 0, false};
        bool target_sequence = tracker.target(tg);
        metrics.on_gaze(gz, target_sequence, tg);
      });

    auto points = eye::target_array(scr, 3, 3);
    tracker.window(points, {500, 1000, 500});
    std::cout << eye::test::line <<'\n'<< "exit" << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

} } // eye::test
//===========================================================================//
