//===========================================================================//
/// @file
/// @brief    @ref eye::Point and @ref eye::PointGroup.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYE_POINT_GROUP_HPP
#define EYE_POINT_GROUP_HPP

#include <deque>      // std::deque
#include <algorithm>  // std::minmax_element

namespace eye {

/// @addtogroup module_analysis
/// @{

/** @brief  Gaze point data.
  @headerfile eye/analysis/point_group.hpp
  @par Header
    @header{eye/analysis/point_group.hpp}

  ...

  eye::Point p{timestamp, x_px, y_px, is_fixation}

  @sa [eyelib-analysis Manual](@ref analysis)
*/
struct Point
{
  // note: C++11 class member initializers
  unsigned  time_ms = 0;  ///< Timestamp in milliseconds.
  double    x_px    = 0;  ///< Horizontal coordinate in pixels.
  double    y_px    = 0;  ///< Vertical coordinate in pixels.
  bool      fix = false;  ///< Fixation flag.
};

//  /** @brief  Converts GazeData to Point.
//    @param  [in]  g   Gaze data object.
//    @return Point object.
//  */
//  inline Point
//  to_point(GazeData const& g)
//  {
//    return {g.time, g.avg.x, g.avg.y, g.avg.x, g.fix};
//  }

//---------------------------------------------------------------------------//

/** @brief  Container for Point elements.
  @headerfile eye/analysis/point_group.hpp
  @par Header
    @header{eye/analysis/point_group.hpp}

  ...

  @sa [eyelib-analysis Manual](@ref analysis)
*/
class PointGroup
{
public:

  /** @brief  Constructs an empty container.
  */
  PointGroup();

  //-----------------------------------------------------------
  /// @name Capacity
  /// @{

  /** @brief  Checks whether the container is empty.
  @return `true` if the container is empty, `false` otherwise.
  */
  bool  empty() const;

  /** @brief  Returns the number points.
  @return The number of points in the container.
  */
  unsigned  size() const;

  /// @}
  //-----------------------------------------------------------
  /// @name Element accesss
  /// @{

  /** @brief  Returns the first point in the container.
  @return First point.
  */
  Point  front() const;

  /// @}
  //-----------------------------------------------------------
  /// @name Modifiers
  /// @{

  /** @brief  Removes all points.
  */
  void  clear();

  /** @brief  Adds a point to the end.
  @param  [in]  p   The point to add.
  */
  void  push_back(Point p);

  /** @brief  Removes the first point.
  */
  void  pop_front();

  /// @}
  //-----------------------------------------------------------
  /// @name Properties
  /// @{

  /** @brief  Returns the horizontal position of the centroid of the points.
  @return `x` coordinate value.
  */
  double  centroid_x() const;

  /** @brief  Returns the vertical position of the centroid of the points.
  @return `y` coordinate value.
  */
  double  centroid_y() const;

  /** @brief  Returns the time duration between the earliest and latest points.
  @return Duration in milliseconds.

  Does not assume time ordered points in the container.
  */
  unsigned  duration_ms() const;

  /** @brief  Gets the fixation flag.
  @return `true` if the container is empty, `false` otherwise.
  */
  bool  fixation() const;

  /** @brief  Sets the fixation flag.
  @param  [in]  f   Fixation flag value.
  */
  void  fixation(bool f);

  /// @}
  //-----------------------------------------------------------

private:
  std::deque<unsigned>    times_;   // double-ended queue of timestamps
  std::deque<eye::Point>  points_;  // double-ended queue of points
  double sum_x_px_;  // running sum of x coordinate values
  double sum_y_px_;  // running sum of y coordinate values
  bool   fixation_;  // fixation flag for the entire group
};

// Inline members -----------------------------------------------------------

inline
PointGroup::PointGroup()
: times_()
, points_()
, sum_x_px_(0)
, sum_y_px_(0)
, fixation_(false)
{}


// Capacity -------------------------------------------------

inline bool
PointGroup::empty() const
{
  return points_.empty();
}

inline unsigned
PointGroup::size() const
{
  return points_.size();
}

// Element access -------------------------------------------

inline Point
PointGroup::front() const
{
  return points_.front();
}

// Modifiers ------------------------------------------------

inline void
PointGroup::clear()
{
  times_.clear();
  points_.clear();
  sum_x_px_ = 0;
  sum_y_px_ = 0;
  fixation = false;
}

inline void
PointGroup::push_back(Point p)
{
  times_.push_back(p.time_ms);
  points_.push_back(p);
  sum_x_ += p.x_px;
  sum_y_ += p.y_px;
}

inline void
PointGroup::pop_front()
{
  if (!points_.empty())
  {
    sum_x_ -= points_[0].x_px;
    sum_y_ -= points_[0].y_px;
    points_.pop_front();
    times_.pop_front();
  }
}

// Properties -----------------------------------------------

inline double
PointGroup::centroid_x() const
{
  if (points_.empty())
  {
    return 0;
  }
  return sum_x_px_ / points_.size();
}

inline double
PointGroup::centroid_y() const
{
  if (points_.empty())
  {
    return 0;
  }
  return sum_y_px_ / points_.size();
}

inline unsigned
PointGroup::duration_ms() const
{
  if (points_.empty())
  {
    return 0;
  }
  //std::deque<unsigned> time_ms;
  // Use transform algorithm with lambda expression
  //std::transform(points_.begin(), points_.end(), std::back_inserter(),
  //               [](eye::Point const& p) { return p.time_ms; });
  auto minmax = std::minmax_element(times_.begin(), times_.end());
  return (*minmax_x.second - *minmax_x.first);
}

inline bool
PointGroup::fixation() const
{
  return fixation_;
}

inline void
PointGroup::fixation(bool f)
{
  fixation_ = f;
}

} // eye

#endif // EYE_POINT_GROUP_HPP
//===========================================================================//
