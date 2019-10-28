//===========================================================================//
/// @file
/// @brief    Fixation metrics.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYELIB_FIXATION_HPP
#define EYELIB_FIXATION_HPP

#include <eyelib/screen.hpp>  // eye::PointXY

//#include <utl/math.hpp>     // utl::math::sample_window

namespace eye {
/// @addtogroup eyelib_gaze
/// @{
//---------------------------------------------------------------------------

/// Fixation object of interest.
class Fixation
{
public:

//  /// Duration sample window: max, min, mean, sum.
//  using duration_values = utl::math::sample_window<unsigned, DurationWindow>;
//
//  /// Interval sample window: max, min, mean, sum.
//  using interval_values = utl::math::sample_window<unsigned, IntervalWindow>;

  /// Constructor.
  Fixation(int radius);

  //-----------------------------------------------------------
  /// @name Object inspection
  /// @{

  std::size_t count() const;        ///< Time duration object has been fixated.
  std::size_t duration() const;     ///< Time duration object has been fixated.
  std::size_t interval() const;     ///< Time duration between fixations.
  bool        is_fixated() const;   ///< Returns true if object is fixated.
  //unsigned    last_time() const;    ///< Last recorded time during fixation.

  int x() const;    ///< Returns X coordinate of object.
  int y() const;    ///< Returns Y coordinate of object.

  /// @}
  //-----------------------------------------------------------
  /// @name Operations
  /// @{

  /// Update gaze data.
  void
  gaze(unsigned time, PointXY<float> gaze, bool fixation);

  /// Update object coordinates.
  void
  position(int x, int y);

  /// @}
  //-----------------------------------------------------------

private:
  std::size_t     count_{0};          // Count of fixations
  std::size_t     duration_{0};       // Time duration object has been fixated
  bool            fixated_{false};    // True if object is fixated
  unsigned        interval_{0};       // Time duration between fixations
  unsigned        last_time_{0};      // Last recorded time during fixation

  PointXY<float>  position_{0.,0.};   // Object coordinates
  float           radius_sq_;         // Radius squared

//  duration_values duration_{};      // Event time duration
//  interval_values interval_{};      // Duration between events
};

//---------------------------------------------------------------------------
/// @}
} // eye
#endif // EYELIB_FIXATION_HPP
//===========================================================================//
