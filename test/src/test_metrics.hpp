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
/// @file
/// @brief    Test eye gaze metrics.
/// @author   Nathan Lucas
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
