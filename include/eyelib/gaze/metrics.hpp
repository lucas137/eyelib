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
/// @brief    Eye gaze metrics.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYELIB_METRICS_HPP
#define EYELIB_METRICS_HPP

#include <eyelib/screen.hpp>  // eye::PointXY

#include <utl/math.hpp>     // utl::math::sample_window

namespace eye {

/// @addtogroup eyelib_gaze
/// @{
//---------------------------------------------------------------------------

/// Time metrics.
template <std::size_t DurationWindow, std::size_t IntervalWindow>
class TimeMetrics
{
public:

  /// Duration sample window: max, min, mean, sum.
  using duration_values = utl::math::sample_window<unsigned, DurationWindow>;

  /// Interval sample window: max, min, mean, sum.
  using interval_values = utl::math::sample_window<unsigned, IntervalWindow>;

  /// Return a const reference to duration sample window.
  duration_values const&
  duration() const
  {
    return duration_;
  }

  /// Return a const reference to interval sample window.
  interval_values const&
  interval() const
  {
    return interval_;
  }

  /// Update time metrics.
  void
  update(unsigned time, bool is_active)
  {
    if (is_active)
    {
      // New duration
      if (!started_)
      {
        if (start_time_ != 0)
        {
          interval_.push(time - start_time_);   // Duration since last event
        }
        start_time_ = time;     // Start of new duration
        started_    = true;
      }
    }
    // End duration
    else if (started_)
    {
      duration_.push(time - start_time_);   // Duration of event
      started_ = false;
    }
  }

private:
  duration_values   duration_{};      // Event time duration
  interval_values   interval_{};      // Duration between events
  unsigned          start_time_{0};   // Timestamp current duration started
  bool              started_{false};  // True if recording duration
};

/**
  @brief Blink time metrics.

  Maximum, minimum, mean, and sum total of:
  - Blink time duration.
  - Interval between blinks.
*/
template <std::size_t DurationWindow, std::size_t IntervalWindow>
using BlinkTime = TimeMetrics<DurationWindow, IntervalWindow>;

/**
  @brief Fixation time metrics.

  Maximum, minimum, mean, and sum total of:
  - Fixation time duration.
  - Interval between fixations.
*/
template <std::size_t DurationWindow, std::size_t IntervalWindow>
using FixationTime = TimeMetrics<DurationWindow, IntervalWindow>;

//---------------------------------------------------------------------------

/// Time metrics.
template <std::size_t SampleWindow>
class SaccadeDistance
{
public:

  /// Distance samples: max, min, mean, sum.
  using distance_values = utl::math::sample_window<unsigned, SampleWindow>;

  /// Return a const reference to distance sample window.
  distance_values const&
  distance_sq() const
  {
    return distance_sq_;
  }

  /// Update saccade distance.
  void
  update(PointXY<float> pt, bool is_active)
  {
    // Continuing saccade
    if (started_)
    {
      // End saccade
      if (!is_active)
      {
        started_  = false;
        auto dx   = (pt.x - start_pt_.x);
        auto dy   = (pt.y - start_pt_.y);
        auto d_sq = ((dx * dx) + (dy * dy));
        distance_sq_.push(static_cast<unsigned>(d_sq));
      }
    }
    // Start new saccade
    else if (is_active)
    {
      start_pt_ = pt;
      started_ = true;
    }
  }

private:
  distance_values   distance_sq_{};
  PointXY<float>    start_pt_{0.0,0.0};
  bool              started_{false};  // True if recording saccade
};

//---------------------------------------------------------------------------

/// Time metrics.
template <std::size_t SampleWindow>
class Pupillometry
{
public:

  /// Pupil size samples: max, min, mean, sum.
  using size_values = utl::math::sample_window<float, SampleWindow>;

  /// Return a const reference to pupil size sample window.
  size_values const&
  pupil_size() const
  {
    return pupil_size_;
  }

  /// Update pupillometry sample window.
  void
  update(float left_size, float right_size)
  {
    pupil_size_.push((left_size + right_size) / 2); // Average pupil size
  }

private:
  size_values   pupil_size_{};
};

//---------------------------------------------------------------------------
/// @}

} // eye

#endif // EYELIB_METRICS_HPP
//===========================================================================//
