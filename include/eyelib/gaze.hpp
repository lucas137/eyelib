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
/// @brief    Eye gaze.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYELIB_GAZE_HPP
#define EYELIB_GAZE_HPP

#include <eyelib/screen.hpp>  // eye::PointXY

#include <eyelib/gaze/metrics.hpp>
#include <eyelib/gaze/point_cluster.hpp>

// Fixation detection algorithms
#include <eyelib/gaze/dispersion_threshold.hpp>
#include <eyelib/gaze/velocity_threshold.hpp>

#include <eyelib/gaze/fixation.hpp>

//#include <utl/json.hpp>     // nlohmann::json

#include <iomanip>    // std::setfill, std::setw
#include <sstream>    // std::ostringstream
#include <ostream>    // std::ostream, std::hex
#include <string>     // std::string

namespace eye {

/**
  @addtogroup eyelib_gaze

  <tt>\#include \<eyelib.hpp\></tt> @a -or- @n
  <tt>\#include \<eyelib/gaze.hpp\></tt>

  Eye gaze data structure and related functions.
*/
/// @{
//---------------------------------------------------------------------------

/// @brief    Eye gaze data
struct Gaze
{
  /// %Pupil coordinates and size.
  struct Pupil
  {
    PointXY<float>  center{0.,0.};  ///< Normalized pupil coordinates.
    float           size{0.0};      ///< %Pupil size.

    /// Construct from values.
    Pupil(PointXY<float> const& center, float size)
    : center(center)
    , size(size)
    {}

    /// Default construct.
    Pupil() = default;
  };

  /** @brief  %Eye tracking state.

  @a bits is a 32-bit masked tracking state:
  ```
  gaze = (bits & 0x01);
  eyes = (bits & 0x02);
  user = (bits & 0x04);
  fail = (bits & 0x08);
  lost = (bits & 0x10);
  ```
  */
  struct Tracking
  {
    unsigned bits{0x00};  ///< Bitfield.
    bool gaze{false};     ///< Calibrated and producing gaze coordinates.
    bool eyes{false};     ///< Possibly calibrated, and tracking both eyes,
                          ///< including pupil and glint.
    bool user{false};     ///< Possibly calibrated, and tracking presence of
                          ///< user.  Presence defined as face or single eye.
    bool fail{false};     ///< Failed to track anything in this frame.
    bool lost{false};     ///< Failed to detect anything, tracking is now lost.

    Tracking(unsigned bits, bool gaze, bool eyes,
             bool user, bool fail, bool lost);    ///< Construct from values.
    Tracking(unsigned bits);                      ///< Construct from bitfield.
    Tracking() = default;                         ///< Default construct.
  };

  std::string     timestamp   {};       ///< `YYYY-MM-DD hh:mm:ss.sss`.
  unsigned        time_ms     {0};      ///< Timestamp in milliseconds.
  Tracking        tracking    {};       ///< Tracking state.
  bool            fixation    {false};  ///< `true` if fixation.
  PointXY<float>  raw_px      {0.,0.};  ///< Raw gaze point.
  PointXY<float>  avg_px      {0.,0.};  ///< Smoothed gaze point.
  Pupil           pupil_left  {};   ///< Left eye pupil coordinates and size.
  Pupil           pupil_right {};   ///< Right eye pupil coordinates and size.

  /// Construct from specified values.
  Gaze(std::string const&     timestamp,
       unsigned               time_ms,
       Tracking const&        tracking,
       bool                   fixation,
       PointXY<float> const&  raw_px,
       PointXY<float> const&  avg_px,
       Pupil const&           pupil_left,
       Pupil const&           pupil_right);

  /// Construct from default values.
  Gaze() = default;
};

//---------------------------------------------------------------------------

template<typename T>
inline std::string
csv_header();

//---------------------------------------------------------------------------

/// @ingroup  eyelib_gaze
/// @brief    Tracking structure serialization header in CSV format.
template<>
inline std::string
csv_header<Gaze::Tracking>()
{
  std::ostringstream oss;
  oss <<      "tracking.bits"   // Bitfield
      <<','<< "tracking.gaze"   // Calibrated, producing data
      <<','<< "tracking.eyes"   // Tracking both eyes
      <<','<< "tracking.user"   // Tracking face or single eye
      <<','<< "tracking.fail"   // Failed to track in this frame
      <<','<< "tracking.lost";  // Tracking lost
  return oss.str();
}

/// @ingroup  eyelib_gaze
/// @brief    Serialize to string in CSV format.
inline std::string
csv(Gaze::Tracking const& t)
{
  std::ostringstream oss;
  oss <<"0x"<< std::hex << std::setfill('0') << std::setw(2) << t.bits
      <<',' << (t.gaze ? "true":"false")    // Calibrated, producing data
      <<',' << (t.eyes ? "true":"false")    // Tracking both eyes
      <<',' << (t.user ? "true":"false")    // Tracking face or single eye
      <<',' << (t.fail ? "true":"false")    // Failed to track in this frame
      <<',' << (t.lost ? "true":"false");   // Tracking lost
  return oss.str();
}

/**
  @ingroup  eyelib_gaze
  @relates  eye::Gaze::Tracking
  @brief    Convert to string (e.g., `0x07:gaze+++`).

  The string contains the tracking state bitfield and a label.

  String          | Description
  ----------------|-----------------------------------------------------
  `0x01:gaze +++` | Calibrated and producing gaze coordinates
  `0x02:eyes ++ ` | Possibly calibrated, and tracking both eyes
  `0x04:user +  ` | Possibly calibrated, and tracking face or single eye
  `0x08:fail -  ` | Failed to track anything in this frame
  `0x10:lost -- ` | Failed to detect anything, tracking is now lost
  `0x**:error---` | Invalid status
*/
inline std::string
to_string(Gaze::Tracking const& t)
{
  std::ostringstream oss;
  oss << "0x" << std::hex << std::setfill('0') << std::setw(2) << t.bits
      << (t.gaze ? ":gaze +++" :
          t.eyes ? ":eyes ++ " :
          t.user ? ":user +  " :
          t.fail ? ":fail -  " :
          t.lost ? ":lost -- " :
                   ":error---");
  return oss.str();
}


/// @name     Non-member function overloads
/// @relates  eye::Gaze::Tracking
/// @{

/// @brief    Insert into output stream.
inline std::ostream&
operator<<(std::ostream& os, Gaze::Tracking const& t)
{
  return os << to_string(t);
}

///@}

//---------------------------------------------------------------------------

/// @name     Non-member function overloads
/// @relates  eye::Gaze
/// @{

/** @brief  Insert into output stream.

  Example:
  ```
  tracking: 0x07:gaze +++  fix:true  raw: 1398,  840   avg: 1382,  819
  ```
*/
inline std::ostream&
operator<<(std::ostream& os, Gaze const& g)
{
  std::ostringstream oss;
  oss << "tracking: " << g.tracking
      << "  fix: "    << std::boolalpha << std::left
                      << std::setw(5)   << g.fixation
      << std::right   << std::fixed     << std::setprecision(0)
      << "  raw:"     << std::setw(5)   << g.raw_px.x << ','
                      << std::setw(5)   << g.raw_px.y
      << "  avg:"     << std::setw(5)   << g.avg_px.x << ','
                      << std::setw(5)   << g.avg_px.y;
return os << oss.str();
}

/// @}

//-----------------------------------------------------------

/// @ingroup  eyelib_gaze
/// @brief    Gaze data structure serialization header in CSV format.
template<>
inline std::string
csv_header<Gaze>()
{
  std::ostringstream oss;
  oss <<      "timestamp"
      <<','<< "time_ms"
      //<<','<< "tracking"
      <<','<< csv_header<Gaze::Tracking>()
      <<','<< "fixation"
      <<','<< "raw_px.x"
      <<','<< "raw_px.y"
      <<','<< "avg_px.x"
      <<','<< "avg_px.y"
      <<','<< "pupil_left.center.x"
      <<','<< "pupil_left.center.y"
      <<','<< "pupil_left.size"
      <<','<< "pupil_right.center.x"
      <<','<< "pupil_right.center.y"
      <<','<< "pupil_right.size";
  return oss.str();
}

/// @ingroup  eyelib_gaze
/// @brief    Serialize to string in CSV format.
inline std::string
csv(Gaze const& g)
{
  std::ostringstream oss;
  oss <<        g.timestamp
      << ',' << g.time_ms
      << ',' << csv(g.tracking)
      << ',' << (g.fixation ? "true":"false")
      << ',' << g.raw_px.x
      << ',' << g.raw_px.y
      << ',' << g.avg_px.x
      << ',' << g.avg_px.y
      << ',' << g.pupil_left.center.x
      << ',' << g.pupil_left.center.y
      << ',' << g.pupil_left.size
      << ',' << g.pupil_right.center.x
      << ',' << g.pupil_right.center.y
      << ',' << g.pupil_right.size;
  return oss.str();
}

///// @ingroup  eyelib_gaze
///// @brief    Convert to JSON object.
//inline nlohmann::json
//json(Gaze const& g)
//{
//  return { { "timestamp", g.timestamp },
//           { "time_ms", g.time_ms },
//           { "tracking",
//             { "gaze", g.tracking.gaze },
//             { "eyes", g.tracking.eyes },
//             { "user", g.tracking.user },
//             { "fail", g.tracking.fail },
//             { "lost", g.tracking.lost } },
//           { "fixation", g.fixation },
//           { "raw_px", { "x", g.raw_px.x }, { "y", g.raw_px.y } },
//           { "avg_px", { "x", g.avg_px.x }, { "y", g.avg_px.y } },
//           { "pupil_left", g.pupil_left },
//           { "pupil_right", g.pupil_right }
//         };
//}

//---------------------------------------------------------------------------
/// @}

} // eye

#endif // EYELIB_GAZE_HPP
//===========================================================================//
