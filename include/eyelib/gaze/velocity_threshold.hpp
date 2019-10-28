//===========================================================================//
/// @file
/// @brief    Velocity threshold fixation detection algorithm.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYE_VELOCITY_THRESHOLD_HPP
#define EYE_VELOCITY_THRESHOLD_HPP

#include <vector>     // std::vector

namespace eye {

/// @addtogroup eyelib_gaze
/// @{

/** @brief  Velocity threshold (VT) fixation detection algorithm.

  The VT algorithm exploits the difference in point-to-point velocity
  observed between fixational and saccadic movements.

  Identifies a fixation as a group of consecutive points, the motion between
  which is within a maximum velocity threshold.  A maximum displacement
  `dmax` is used as a proxy for the velocity threshold.
*/
class VelocityThreshold
{
public:  //-----------------------------------------------------------

  /**
  @brief  Construct a fixation detection object.
  @param  [in]  dmax  Maximum displacement in pixels (velocity proxy)
                      between fixation points.
  */
  VelocityThreshold(float dmax);

  /**
  @brief  Add a gaze point.
  @param  [in]  x   %Gaze point X coordinate.
  @param  [in]  y   %Gaze point Y coordinate.
  @return `true` if a fixation is detected, `false` otherwise.
  */
  bool
  fixation(float x, float y);

  /**
  @brief  Returns the current fixation point, and the
          number of gaze points within the fixation cluster.
  @param  [out] x   Horizontal coordinate.
  @param  [out] y   Vertical coordinate.
  @param  [out] n   Number of points in the fixation cluster, or `0` if
                    the last point has not been identified with a fixation.

  The fixation point is computed as the centroid
  of the gaze points within the fixation cluster.
  */
  void
  centroid(double& x, double& y, unsigned& n) const;

  /// Returns displacement between points (velocity proxy).
  float
  displacement() const;

private:  //-----------------------------------------------------------

  float d_sq_max_;        // Maximum displacement squared of fixation points
  float d_sq_{0};         // Displacement squared of fixation points
  bool  is_fix_{false};   // True if group of points is a fixation

  std::vector<float> x_{};    // x coordinates
  std::vector<float> y_{};    // y coordinates
};

/// @}

} // eye

#endif // EYE_VELOCITY_THRESHOLD_HPP
//===========================================================================//
