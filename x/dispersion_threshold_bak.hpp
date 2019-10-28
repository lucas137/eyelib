//===========================================================================//
/// @file
/// @brief    @ref eye::DispersionThreshold algorithm.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYE_DISPERSION_THRESHOLD_HPP
#define EYE_DISPERSION_THRESHOLD_HPP

#include <deque>      // std::deque
#include <algorithm>  // std::minmax_element

namespace eye {

/// @addtogroup module_fixation
/// @{

/** @brief  Dispersion threshold (DT) fixation detection algorithm.
  @headerfile eye/dispersion_threshold.hpp
  @par Header
    @header{eye/dispersion_threshold.hpp}

  Identifies a fixation as a group of consecutive points within a dispersion,
  or maximum separation, that spans a minimum duration.

  @sa [eyelib-analysis Manual](@ref fixation_dt)
*/
class DispersionThreshold
{
public:

  /**
  @brief  Constructs a gaze data processing object.
  @param  [in]  min_pt  Minimum number of points in the moving window.
  @param  [in]  max_px  Maximum dispersion in pixels of fixation points.
  */
  /*inline*/
  DispersionThreshold(unsigned min_pt, double max_px);

  /**
  @brief  Add a gaze point.
  @param  [in]  x   Horizontal coordinate.
  @param  [in]  y   Vertical coordinate.
  */
  /*inline*/ void
  push_back(double x, double y);

  /**
  @brief  Checks if the last point added is part of a fixation cluster.
  @return `true` if the last point belongs to a fixation, `false` otherwise.

  The minimum window condition must be met before a point can be identified
  with a fixation cluster.  See the constructor parameter <em>min_pt</em>.
  That means this method will only return `true` if at least
  <em>min_pt</em> points have been collected since object instantiation
  or the last fixation terminated.

  @note Consecutive points within the dispersion
        threshold represent the same fixation.
  */
  /*inline*/ bool
  is_fixation() const;

  /**
  @brief  Returns the current fixation point, and the
          number of gaze points within the fixation cluster.
  @param  [out] x   Horizontal coordinate.
  @param  [out] y   Vertical coordinate.
  @param  [out] n   Number of points in the fixation cluster, or `0` if
                    the last point has not been identified with a fixation.

  The fixation point is computed as the centroid
  of the gaze points within the fixation cluster.
  @note See the minimum window condition described at `fixation()`.
  */
  /*inline*/ Fixation
  fixation() const;

private:  //-----------------------------------------------------------

  unsigned  min_pt_;
  double    max_px_;
  bool      fixation_;
  std::deque<double> x_, y_;
  double    max_x_, min_x_, max_y_, min_y_;


  eye::PointGroup points_;
};

/// @}

//---------------------------------------------------------------------------//

inline
DispersionThreshold::DispersionThreshold(unsigned min_pt, double max_px)
: min_pt_(min_pt)
, max_px_(max_px)
, fixation_(false)
, x_()
, y_()
, max_x_(0.0)
, min_x_(0.0)
, max_y_(0.0)
, min_y_(0.0)
{}

inline void
DispersionThreshold::push_back(double x, double y)
{
  // The following statements avoid an assignment operation if the min or
  // max value does not change.  Alternatively, each call to std::max and
  // std::min may perform an assignment, even if there is no change to the
  // min and max value respectively.
  if (x > max_x_) { max_x_ = x }
  if (x < min_x_) { min_x_ = x }
  if (y > max_y_) { max_y_ = y }
  if (y < min_y_) { min_y_ = y }

  x_.push_back(x);
  y_.push_back(y);

  points_.push_back(p);

  // Duration threshold is implemented as a minimum
  // number of points within a fixation cluster.
  if (x_.size() < min_pt_)
  {
    // Continue accumulating points until duration threshold is met
    return;
  }

  // Dispersion threshold is implemented as a maximum dispersion in pixels.
  double d = ((max_x_ - min_x)_ + (max_y_ - min_y_));

  // If `d` is within dispersion threshold `max_px`, the cluster of
  // points in the window are considered to represent a fixation.
  if (d <= max_px_)
  {
    // Keep adding points until dispersion threshold is exceeded.
    fixation_ = true;
    return;
  }

  // Exceeded threshold.
  if (fixation_)
  {
    // If a fixation occurred, reset all variables, but keep
    // the current point as the first point in a new window.
    x_.clear();       // Remove all points.
    y_.clear();
    x_.push_back(x);  // Re-add non-fixation point.
    y_.push_back(y);
    max_x_ = x;
    min_x_ = x;
    max_y_ = y;
    min_y_ = y;
    fixation_ = false;
    return;
  }

  x_.pop_front();   // Remove first point in window.
  y_.pop_front();

  // Re-compute min and max values.
  auto minmax_x = std::minmax_element(x_.begin(), x_.end());
  min_x_ = *minmax_x.first;
  max_x_ = *minmax_x.second;
  auto minmax_y = std::minmax_element(y_.begin(), y_.end());
  min_y_ = *minmax_y.first;
  max_y_ = *minmax_y.second;
}


inline bool
DispersionThreshold::is_fixation() const
{
  return fixation_;
}


inline Fixation
DispersionThreshold::fixation() const
{
  Fixation f;
  if (fixation_)
  {
    // Verify coordinate vectors are not empty.
    if (x_.empty() || y_.empty())
    {
      return;   // empty vector
    }
    // Verify equal vector sizes.
    if (x_.size() != y_.size())
    {
      return;   // unequal vector sizes
    }
    // Number of points in fixation cluster.
    f.n = x_.size();
    // Compute centroid of cluster points.
    double sum_x = std::accumulate(x_.begin(), x_.end(), 0.0);
    f.x = sum_x / f.n;
    double sum_y = std::accumulate(y_.begin(), y_.end(), 0.0);
    f.y = sum_y / f.n;
  }
  return f;
}


} // eye

#endif // EYE_DISPERSION_THRESHOLD_HPP
//===========================================================================//
