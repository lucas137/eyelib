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
/// @brief    Test fixation detection algorithms.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYELIB_TEST_FIXATION_HPP
#define EYELIB_TEST_FIXATION_HPP

#include <eyelib.hpp>

#include <utl/file.hpp>     // utl::file::file_writer

#include <vector>       // std::vector

namespace eye { namespace test {
//---------------------------------------------------------------------------
/// @addtogroup eyelib_test
/// @{

class FixationTest
{
public:
  /// Constructor.
  FixationTest(eye::Targets const& targets,
               std::string const& datetime_basic,
               std::string const& datetime_extended);

  /// Destructor.
  ~FixationTest();

  /// Process eye gaze data.
  void on_gaze(eye::Gaze const& gz, bool ts, eye::Target const& tg);

private:
  utl::file::file_writer  data_log_{};    // Data log file

  // Dispersion threshold parameters
  static constexpr unsigned DTN = 4;      // Number of points in moving window
  static constexpr float    DTD = 30.0;   // Maximum dispersion in pixels
                                          // of fixation points
  // Velocity threshold parameter
  static constexpr float    VTD = 7.0;    // Maximum displacement in pixels
                                          // between fixation points
  // Fixation detection algorithms
  eye::DispersionThreshold  dt_{DTN, DTD};  // Dispersion threshold
  eye::VelocityThreshold    vt_{VTD};       // Velocity threshold

  // Fixation metrics
  static constexpr int        RADIUS = 56;
//  std::vector<eye::Fixation>  fixations_; // Fixation on objects of interest
  std::vector<eye::Fixation>  et_fixations_; // Fixation on objects of interest
  std::vector<eye::Fixation>  dt_fixations_; // Fixation on objects of interest
  std::vector<eye::Fixation>  vt_fixations_; // Fixation on objects of interest
};

//-----------------------------------------------------------

/// Test fixation detection algorithms.
void
fixation(unsigned screen_index);

/// @}
//---------------------------------------------------------------------------
} } // eye::test

#endif // EYELIB_TEST_FIXATION_HPP
//===========================================================================//
