//===========================================================================//
/// @file
/// @brief    Eye tracker calibration results.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYELIB_CALIBRATION_HPP
#define EYELIB_CALIBRATION_HPP

#include <eyelib/screen.hpp>  // eye::Point

#include <ostream>    // std::ostream
#include <string>     // std::string
#include <vector>     // std::vector

namespace eye {

//---------------------------------------------------------------------------
/**
  @addtogroup eyelib_calib

  <tt>\#include \<eyelib.hpp\></tt> @a -or- @n
  <tt>\#include \<eyelib/calibration.hpp\></tt>

  Eye tracker calibration results.
*/
/// @{

//struct CriteriaPair
//{
//  float avg;  // average
//  float eye;  // each eye
//
////  CriteriaPair() : avg(), eye() {}
//
//  CriteriaPair() = default;   // ?
//
//  CriteriaPair(float avg, float eye) : avg(avg), eye(eye) {}
//};
//
//struct Criteria
//{
//  CriteriaPair average_error_deg{ 0.0, 0.0 };
//  CriteriaPair point_accuracy_deg{ 0.0, 0.0 };
//  CriteriaPair point_mean_err_px{ 0.0, 0.0 };
//  CriteriaPair point_std_dev_px{ 0.0, 0.0 };
//
//  CriteriaPair ... //
//};

/// %Calibration results.
struct Calibration
{
  /// Generic data structure.
  template<typename T>
  struct Eyes
  {
    T binocular;    ///< Both eyes.
    T left_eye;     ///< Left eye.
    T right_eye;    ///< Right eye.
  };

  /// %Calibration quality rating.
  enum class Rating : unsigned
  {
    uncalibrated = 0, ///< Not calibrated.
    recalibrate  = 1, ///< Average error ≥ 1.5 degree.
    poor         = 2, ///< Average error < 1.5 degree.
    moderate     = 3, ///< Average error < 1.0 degree.
    good         = 4, ///< Average error < 0.7 degree.
    great        = 5  ///< Average error < 0.5 degree.
  };

  /// %Calibration point results.
  struct Point
  {
    /// Sample state.
    enum class State : unsigned
    {
      no_data  = 0,   ///< No useful tracking data gathered. Resample required.
      resample = 1,   ///< Data gathered is questionable quality.
      ok       = 2,   ///< The calibration point is okay. No resample required.
      error    = 3,   ///< Invalid state code: value out of range.
    };

    //CalibPoint() = default;

    State           sample_state{State::no_data};   ///< Sample state.
    PointXY<unsigned> calibrate_px{0,0};      ///< %Calibration coordinates.
    PointXY<float>  estimated_px{0.,0.};      ///< Mean estimated coordinates.
    Eyes<float>     accuracy_deg{0.,0.,0.};   ///< Accuracy in degrees.
    Eyes<Rating>    accuracy_rating{};        ///< Accuracy rating.
    Eyes<float>     mean_error_px{0.,0.,0.};  ///< Mean error in pixels.
    Eyes<float>     avg_std_dev_px{0.,0.,0.}; ///< Average standard deviation in pixels.
  };

  /// %Calibration point container.
  using Points = std::vector<Point>;

  bool         success{false};      ///< `true` if calibration was successful.
  Eyes<float>  error_deg{0.,0.,0.}; ///< Average error in degrees.
  Eyes<Rating> error_rating{};      ///< Average error rating.
  Points       points{};            ///< %Calibration point results.
};

/// @}
//---------------------------------------------------------------------------

/// @relates  eye::Calibration
/// @{

/** @brief    Convert to string.

  ```
  success     : false
  binocular   : 0.0000 deg : uncalibrated
  left_eye    : 0.0000 deg : recalibrate
  right_eye   : 0.0000 deg : poor
  calibpoints :
        sample      calibrate    estimated    accuracy   mean_err   std_dev
        state       (pixel)      (pixel)      (degree)   (pixel)    (pixel)
        -------------------------------------------------------------------
    0:  no_data     0000,0000    0000,0000    00.0000    00.0000    00.0000
    1:  resample    0000,0000    0000,0000    00.0000    00.0000    00.0000
    2:  ok          0000,0000    0000,0000    00.0000    00.0000    00.0000
    …   …              …,   …       …,   …          …          …          …
   NN:  error       0000,0000    0000,0000    00.0000    00.0000    00.0000
        -------------------------------------------------------------------
  ```
*/
std::string
to_string(Calibration const& c);

/// @brief    Insert into output stream.
std::ostream&
operator<<(std::ostream& os, Calibration const& c);

/// @brief    Serialize to string in CSV format.
std::string
csv(Calibration const& c);

///@}

//---------------------------------------------------------------------------

/// @relates  eye::Calibration::Point
/// @{

/// @brief    Compute the centroid of calibration coordinates in `pts`.
void
centroid(unsigned& x, unsigned& y, Calibration::Points const& pts);

///@}

//---------------------------------------------------------------------------

} // eye

#endif // EYELIB_CALIBRATION_HPP
//===========================================================================//
