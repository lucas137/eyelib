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
/// @brief    Dispersion threshold fixation detection algorithm.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYE_DISPERSION_THRESHOLD_HPP
#define EYE_DISPERSION_THRESHOLD_HPP

#include <deque>      // std::deque

namespace eye {

/// @addtogroup eyelib_gaze
/// @{

/** @brief  Dispersion threshold (DT) fixation detection algorithm.

  The DT algorithm defines a fixation as a group of consecutive points
  within some dispersion threshold (maximum separation).  The group, or
  cluster, of fixation point samples must span some minimum time duration.

  This implementation defines duration in terms of the number of points
  received, with the assumption that points are collected at a known
  constant sampling rate.  The `fixation` method accepts eye gaze point
  coordinates until a window of `pts` points is collected, and returns
  `true` if a fixation is detected and `false` if not.  Note that to
  detect a fixation at least `pts` points must be collected since object
  instantiation or the last fixation ended.

  When the point window is filled, the dispersion of the points is computed
  and compared with the maximum dispersion specified by the threshold
  parameter `dmax`.  If the dispersion exceeds the threshold, the first
  (oldest) point in the window is removed in order to move the window in
  preparation for a new point to be added with the next call to `fixation`.

  If the dispersion is less than or equal to the threshold, `fixation`
  returns `true` to indicate a fixation is detected.  Once a fixation is
  detected, subsequent points within the dispersion threshold are added
  to the fixation group.  During a fixation, no points are removed.

  Upon a point exceeding the threshold, the fixation points are removed
  and the current point is added to a new detection window.  Then points
  must be collected to fill the window with `pts` points before checking
  for a new fixation.
*/
class DispersionThreshold
{
public:  //-----------------------------------------------------------

  /**
  @brief  Construct a fixation detection object.
  @param  [in]  pts   Number of points in the moving window.
  @param  [in]  dmax  Maximum dispersion of fixation points.
  */
  DispersionThreshold(unsigned pts, float dmax);

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

  /// Returns dispersion of points within the moving window.
  float
  dispersion() const;

private:  //-----------------------------------------------------------

  unsigned  pt_min_;        // Minimum number of points in a fixation group
  float     d_max_;         // Maximum dispersion of fixation group points
  float     d_{0};          // Dispersion of points
  bool      is_fix_{false}; // True if group of points is a fixation

  // Double-ended queues
  std::deque<float> x_{};   // x coordinates
  std::deque<float> y_{};   // y coordinates
};

/// @}

} // eye

#endif // EYE_DISPERSION_THRESHOLD_HPP
//===========================================================================//
