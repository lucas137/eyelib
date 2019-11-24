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

#include <eyelib/gaze.hpp>

namespace eye {
//---------------------------------------------------------------------------

Gaze::Tracking::Tracking(unsigned bits, bool gaze, bool eyes,
                         bool user, bool fail, bool lost)
: bits(bits), gaze(gaze), eyes(eyes), user(user), fail(fail), lost(lost)
{}

Gaze::Tracking::Tracking(unsigned bits)
: bits(bits)
, gaze(bits & 0x01)
, eyes(bits & 0x02)
, user(bits & 0x04)
, fail(bits & 0x08)
, lost(bits & 0x10)
{}

//---------------------------------------------------------------------------

Gaze::Gaze(std::string const& timestamp, unsigned time_ms,
           Tracking const& tracking, bool fixation,
           PointXY<float> const& raw_px, PointXY<float> const& avg_px,
           Pupil const& pupil_left, Pupil const& pupil_right)
: timestamp(timestamp)
, time_ms(time_ms)
, tracking(tracking)
, fixation(fixation)
, raw_px(raw_px)
, avg_px(avg_px)
, pupil_left(pupil_left)
, pupil_right(pupil_right)
{}

//---------------------------------------------------------------------------
} // eye
//===========================================================================//
