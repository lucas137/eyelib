//===========================================================================//
//  Nathan Lucas
//  2016
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
