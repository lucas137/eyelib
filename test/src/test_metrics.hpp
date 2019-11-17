//===========================================================================//
/// @file
/// @brief    Test eye gaze metrics.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYELIB_TEST_METRICS_HPP
#define EYELIB_TEST_METRICS_HPP

#include <eyelib.hpp>

#include <utl/file.hpp>     // utl::file::file_writer

namespace eye { namespace test {
//---------------------------------------------------------------------------
/// @addtogroup eyelib_test
/// @{

class GazeMetrics
{
public:
  /// Constructor.
  GazeMetrics();

  /// Process eye gaze data.
  void on_gaze(eye::Gaze const& gz, bool ts, eye::Target const& tg);

private:
  utl::file::file_writer  log_file_{};          // Data log

  // Dispersion threshold parameters
  static constexpr unsigned DTN = 10;     // Number of points in moving window
  static constexpr float    DTD = 72.0;   // Max dispersion of points

  // Velocity threshold parameter
  static constexpr float    VTD = 10.0;   // Max displacement between points

  // Fixation detection algorithms
  eye::DispersionThreshold  dt{DTN, DTD};
  eye::VelocityThreshold    vt{VTD};

  /*
    Anticipated average blink rate between 4.5 and 32.5 blinks/min  (??)
  */
  static constexpr std::size_t FrameRate      = 60; // Eye tracker frame rate
  static constexpr std::size_t SampleSeconds  = 5;  // Sample window duration
  static constexpr std::size_t SampleWindow   = (SampleSeconds*FrameRate);

  // General metrics
  eye::BlinkTime<SampleWindow, SampleWindow>    blink{};      // Blink time metrics
  eye::FixationTime<SampleWindow, SampleWindow> fixation{};   // Fixation time metrics
  eye::SaccadeDistance<SampleWindow>            saccade{};    // Saccade distance
  eye::Pupillometry<SampleWindow>               pupil{};      // Pupil size
};

//-----------------------------------------------------------

/// Test eye tracker gaze data processing algorithms.
void
metrics(unsigned screen_index);

/// @}
//---------------------------------------------------------------------------
} } // eye::test

#endif // EYELIB_TEST_METRICS_HPP
//===========================================================================//
