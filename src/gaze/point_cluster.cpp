//===========================================================================//
//  Nathan Lucas
//  2016
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
