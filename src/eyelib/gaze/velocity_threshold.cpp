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
#include <eyelib/gaze/velocity_threshold.hpp>

#include <cmath>    // std::sqrt
#include <numeric>  // std::accumulate

namespace {   //-------------------------------------------------------------
} // anonymous --------------------------------------------------------------

namespace eye {


VelocityThreshold::VelocityThreshold(float dmax)
: d_sq_max_(dmax * dmax)
{}


bool
VelocityThreshold::fixation(float x, float y)
{
  // Add the current point
  x_.push_back(x);
  y_.push_back(y);

  // Must collect at least points
  if (x_.size() < 2)
  {
    d_sq_ = 0;
    return false;
  }

  // Get last point before current point
  auto xp = x_[x_.size() - 2];
  auto yp = y_[y_.size() - 2];

  // Threshold is implemented as a maximum squared displacement in pixels
  d_sq_ = ((x - xp) * (x - xp) +
           (y - yp) * (y - yp));

  // If the displacement squared (proxy for velocity) is within
  // threshold, the points represent a fixation; reject a value
  // of zero, as it indicates invalid coordinates
  if ((d_sq_ > 0) && (d_sq_ <= d_sq_max_))
  {
    // Continue adding points until velocity threshold is exceeded
    is_fix_ = true;
    return true;
  }

  // Velocity threshold was exceeded
  is_fix_ = false;

  // Remove all points
  x_.clear();
  y_.clear();

  // Re-add current point
  x_.push_back(x);
  y_.push_back(y);

  return false;
}


void
VelocityThreshold::centroid(double& x, double& y, unsigned& n) const
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
VelocityThreshold::displacement() const
{
  return (d_sq_ < 0) ? 0 : std::sqrt(d_sq_);
}


} // eye
//===========================================================================//
