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

#include <eyelib/gaze/point_cluster.hpp>

#include <numeric>    // std::accumulate

namespace eye {
//---------------------------------------------------------------------------

void
PointCluster::clear()
{
  t_.clear();
  x_.clear();
  y_.clear();
}

unsigned
PointCluster::duration() const
{
  if (t_.back() < t_.front()) { return 0; }
  return t_.back() - t_.front();
}

bool
PointCluster::empty() const
{
  return x_.empty();
}

float
PointCluster::mean_x() const
{
  return std::accumulate(x_.cbegin(), x_.cend(), 0.0) / x_.size();
}

float
PointCluster::mean_y() const
{
  return std::accumulate(y_.cbegin(), y_.cend(), 0.0) / y_.size();
}

void
PointCluster::pop_front()
{
  t_.pop_front();
  x_.pop_front();
  y_.pop_front();
}

void
PointCluster::push_back(unsigned t, float x, float y)
{
  t_.push_back(t);
  x_.push_back(x);
  y_.push_back(y);
}

std::size_t
PointCluster::size() const
{
  return x_.size();
}

void
PointCluster::start(unsigned t)
{
  t_.push_back(t);
}

//---------------------------------------------------------------------------
} // eye
//===========================================================================//
