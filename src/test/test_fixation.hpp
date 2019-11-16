//===========================================================================//
/// @file
/// @brief    Test fixation detection algorithms.
/// @author   Nathan Lucas
/// @date     2016
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
