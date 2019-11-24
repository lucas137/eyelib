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
/// @brief    Eye gaze point cluster.
/// @author   Nathan Lucas
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
