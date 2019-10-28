//===========================================================================//
/// @file
/// @brief    Eye gaze point cluster.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYELIB_POINT_CLUSTER_HPP
#define EYELIB_POINT_CLUSTER_HPP

#include <deque>      // std::deque

namespace eye {

/// @addtogroup eyelib_gaze
/// @{

/// @brief    Group of gaze point coordinates
/// @ingroup  eyelib_gaze
class PointCluster
{
public:
  void      clear();          ///< Clear content.
  unsigned  duration() const; ///< Time duration between first and last point.
  bool      empty() const;    ///< Return `true` if no points.

  float     mean_x() const;   ///< Return mean average x coordinate.
  float     mean_y() const;   ///< Return mean average y coordinate.

  void  pop_front();                              ///< Delete the first point.
  void  push_back(unsigned t, float x, float y);  ///< Add point at the end.

  std::size_t size() const;   ///< Return number of points.

  void  start(unsigned t);    ///< Start time.

private:
  // Double-ended queues
  std::deque<unsigned>  t_{};   // Timestamps
  std::deque<float>     x_{};   // x coordinates
  std::deque<float>     y_{};   // y coordinates
};

/// @}

} // eye

#endif // EYELIB_POINT_CLUSTER_HPP
//===========================================================================//
