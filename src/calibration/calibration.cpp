//===========================================================================//
//  Nathan Lucas
//  2016
//===========================================================================//

#include <eyelib/calibration.hpp>

//#include "calibration/calib_eyes.hpp"
#include "calibration/calib_point.hpp"
#include "calibration/calib_rating.hpp"

#include <algorithm>  // std::max, std::min
#include <sstream>    // std::ostringstream
#include <ostream>    // std::ostream
#include <string>     // std::string

namespace eye {

//---------------------------------------------------------------------------

std::string
to_string(Calibration const& c)
{
  std::ostringstream oss(std::ostringstream::ate);  // write at end

  oss <<   "success         : " << (c.success ? "true" : "false")
      << std::setprecision(4)   << std::fixed
      << "\nerror.binocular : " << std::setw(6) << c.error_deg.binocular
                            << " deg : " <<     c.error_rating.binocular
      << "\nerror.left_eye  : " << std::setw(6) << c.error_deg.left_eye
                            << " deg : " <<     c.error_rating.left_eye
      << "\nerror.right_eye : " << std::setw(6) << c.error_deg.right_eye
                            << " deg : " <<     c.error_rating.right_eye
      << "\ncalibpoints:\n"
      << "\n      sample      calibrate    estimated    accuracy   mean_err   std_dev"
      << "\n      state       (pixel)      (pixel)      (degree)   (pixel)    (pixel)"
      << "\n      -------------------------------------------------------------------";

  std::size_t n = 0;
  for (auto const& p : c.points)
  {
    oss << '\n' << std::setw(3) << n++ << ":  " << p;
  }
  oss << "\n      -------------------------------------------------------------------\n";
  return oss.str();
}

std::ostream&
operator<<(std::ostream& os, Calibration const& c)
{
  return os << eye::to_string(c);
}

std::string
csv(Calibration const& c)
{
  std::ostringstream oss(std::ostringstream::ate);  // write at end

  oss << "calibration.success"  << ',' << (c.success ? "true" : "false")

      <<'\n'<< "calibration.error"  <<','<< "error_deg"
                                    <<','<< "error_rating"
      <<'\n'<< ".binocular" <<','<< c.error_deg.binocular
                            <<','<< c.error_rating.binocular
      <<'\n'<< ".left_eye"  <<','<< c.error_deg.left_eye
                            <<','<< c.error_rating.left_eye
      <<'\n'<< ".right_eye" <<','<< c.error_deg.right_eye
                            <<','<< c.error_rating.right_eye

      <<'\n'<< "calibration.points"  <<','<< "sample_state"
            <<','<< "calibrate_px"   <<','
            <<','<< "estimated_px"   <<','
            <<','<< "accuracy_deg"   <<','               <<','
            <<','<< "accuracy_rating"<<','               <<','
            <<','<< "mean_error_px"  <<','               <<','
            <<','<< "avg_std_dev_px" <<','               <<','
      <<'\n'                         <<','
            <<','<< ".x"             <<',' << ".y"
            <<','<< ".x"             <<',' << ".y"
            <<','<< ".binocular"     <<','<< ".left_eye" <<','<< ".right_eye"
            <<','<< ".binocular"     <<','<< ".left_eye" <<','<< ".right_eye"
            <<','<< ".binocular"     <<','<< ".left_eye" <<','<< ".right_eye"
            <<','<< ".binocular"     <<','<< ".left_eye" <<','<< ".right_eye";

  std::size_t n = 0;
  for (auto const& p : c.points)
  {
    oss << '\n' << "calibration.points[" << n++ << "]" <<','<< eye::csv(p);
  }
  return oss.str();
}

#if 0
/// @ingroup  eyelib_gaze
/// @brief    Convert to JSON object.
inline nlohmann::json
json(Calibration const& c)
{
  return { { "success", c.success },
           { "error_deg", c.error_deg },
           { "error_left_deg", c.error_left_deg },
           { "error_right_deg", c.error_right_deg },
           { "points", c.points }
         };
}
#endif


//---------------------------------------------------------------------------

void
centroid(unsigned& x, unsigned& y, Calibration::Points const& pts)
{
  unsigned max_x = 0;
  unsigned min_x = 1920;
  unsigned max_y = 0;
  unsigned min_y = 1200;

  for (auto const& p : pts)
  {
    min_x = std::min(p.calibrate_px.x, min_x);
    max_x = std::max(p.calibrate_px.x, max_x);
    min_y = std::min(p.calibrate_px.y, min_y);
    max_y = std::max(p.calibrate_px.y, max_y);
  }
  x = min_x + ((max_x - min_x) / 2);
  y = min_y + ((max_y - min_y) / 2);
}

//---------------------------------------------------------------------------


} // eye
//===========================================================================//
