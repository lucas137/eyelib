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

#include <eyelib/gaze/fixation.hpp>

#include <iostream>   // tmp

namespace {   //-------------------------------------------------------------

float
distance_squared(eye::PointXY<float> pt1, eye::PointXY<float> pt2)
{
  auto dx = (pt2.x - pt1.x);
  auto dy = (pt2.y - pt1.y);
  return ((dx*dx) + (dy*dy));
}

} // anonymous --------------------------------------------------------------

namespace eye {

Fixation::Fixation(int radius)
: radius_sq_(radius * radius)
{}

//---------------------------------------------------------------------------

std::size_t Fixation::count() const       { return count_; }
std::size_t Fixation::duration() const    { return duration_; }
std::size_t Fixation::interval() const    { return interval_; }
bool        Fixation::is_fixated() const  { return fixated_; }
//unsigned    Fixation::last_time() const   { return last_time_; }

// Round and cast to integer
int Fixation::x() const { return static_cast<int>(position_.x + 0.5); }
int Fixation::y() const { return static_cast<int>(position_.y + 0.5); }

//---------------------------------------------------------------------------

void
Fixation::gaze(unsigned time, PointXY<float> gaze, bool fixation)
{
  if (fixation)
  {
    auto distance_sq = distance_squared(position_, gaze);

    // Fixation within object radius
    if (distance_sq < radius_sq_)
    {
      // New fixation on object
      if (!fixated_)
      {
        fixated_    = true;
        //std::cout << "fixation start near (" << position_.x << ", "
        //                                     << position_.y << ")\n";
        count_++;   // Count of fixations on object

        if (last_time_ != 0)
        {
          interval_ = (time - last_time_);
          //interval_.push(time - start_time_);   // Duration since last event
        }

        // Response time until first fixation (with respect to what starting point?)

      }
      // Continuing fixation
      else
      {
        duration_ += (time - last_time_);
      }
      last_time_ = time;  // Start time of new duration
      return;             // Done processing
    }
    // Fixation not within object radius, keep processing...
  }
  // No current fixation on object:
  // If object was previously fixated, end duration
  if (fixated_)
  {
    fixated_ = false;
    //std::cout << "fixation ended\n";
    duration_ += (time - last_time_);
    last_time_ = time;  // Start time of interval between fixations
  }
}

void
Fixation::position(int x, int y)
{
  position_.x = x;
  position_.y = y;
}

} // eye
//===========================================================================//
