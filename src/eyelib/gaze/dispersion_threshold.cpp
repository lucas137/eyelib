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

#include <eyelib/gaze/dispersion_threshold.hpp>

#include <numeric>    // std::accumulate
#include <algorithm>  // std::minmax_element

namespace eye {


DispersionThreshold::DispersionThreshold(unsigned pts, float dmax)
: pt_min_(pts)
, d_max_(dmax)
{}


bool
DispersionThreshold::fixation(float x, float y)
{
  // Add the current point
  x_.push_back(x);
  y_.push_back(y);

  // The duration threshold is implemented as a
  // minimum number of points within a cluster
  if (x_.size() < pt_min_)
  {
    d_ = 0;
    return false;   // Continue adding points until the minimum is reached
  }

  // Compute dispersion of the points
  auto xe = std::minmax_element( std::begin(x_), std::end(x_) );
  auto ye = std::minmax_element( std::begin(y_), std::end(y_) );
  d_  = ((*xe.second - *xe.first) + (*ye.second - *ye.first));

  // If points are within dispersion threshold `max_px`,
  // they are considered to represent a fixation; reject
  // a value of zero, as it indicates invalid coordinates
  if ((d_ > 0) && (d_ <= d_max_))
  {
    is_fix_ = true;
    return true;    // Continue adding points until threshold is exceeded
  }

  // Dispersion threshold was exceeded

  // If the group of points contained a fixation, clear all the
  // points and start a new window with only the current point
  if (is_fix_)
  {
    is_fix_ = false;
    x_.clear();
    y_.clear();
    x_.push_back(x);
    y_.push_back(y);
  }
  // If a fixation has not been detected, remove the first
  // point to move the window before adding the next point
  else
  {
    x_.pop_front();
    y_.pop_front();
  }
  return false;
}


void
DispersionThreshold::centroid(double& x, double& y, unsigned& n) const
{
  if (is_fix_)
  {
    if (x_.empty() || y_.empty()) { return; }   // empty vector(s)
    if (x_.size() != y_.size())   { return; }   // unequal vector sizes

    // Number of points in fixation cluster
    n = x_.size();

    // Compute centroid of cluster points
    double sum_x = std::accumulate(x_.begin(), x_.end(), 0.0);
    x = sum_x / n;
    double sum_y = std::accumulate(y_.begin(), y_.end(), 0.0);
    y = sum_y / n;
  }
}


float
DispersionThreshold::dispersion() const
{
  return d_;
}


} // eye
//===========================================================================//
