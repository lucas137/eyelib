//===========================================================================//
//  Nathan Lucas
//  2016
//===========================================================================//

#include "tracker/message.hpp"

#include "calibration/calib_point.hpp"
#include "calibration/calib_rating.hpp"
#include "debug/debug_out.hpp"

#include <utl/json.hpp>   // nlohmann::json

#include <array>      // std::array
//#include <iostream>   // std::cout
#include <exception>  // std::exception

namespace {   //-------------------------------------------------------------

// Exceptions for all parsing functions:
//  std::domain_error if a JSON value is not an object of the correct type.
//  std::out_of_range if a specified key is not stored in j.

//-----------------------------------------------------------

eye::Calibration::Eyes<float>
eyes(nlohmann::json const& eyes, std::array<std::string,3> const& keys)
{
  float binocular = eyes.at(keys[0]);
  float left_eye  = eyes.at(keys[1]);
  float right_eye = eyes.at(keys[2]);
  return {binocular, left_eye, right_eye};
}

//-----------------------------------------------------------

template<typename T>
eye::PointXY<T>
xy(nlohmann::json const& point);

template<>
eye::PointXY<float>
xy<float>(nlohmann::json const& point)
{
  float x  = point.at("x");
  float y  = point.at("y");
  return {x, y};
}

template<>
eye::PointXY<unsigned>
xy<unsigned>(nlohmann::json const& point)
{
  auto x  = static_cast<unsigned>(point.at("x"));
  auto y  = static_cast<unsigned>(point.at("y"));
  return {x, y};
}

//-----------------------------------------------------------

eye::Gaze::Pupil
pupil(nlohmann::json const& eye_data)
{
  nlohmann::json pcenter = eye_data.at("pcenter");
  float x    = pcenter.at("x");
  float y    = pcenter.at("y");
  float size = eye_data.at("psize");
  return {{x, y}, size};
}

} // anonymous --------------------------------------------------------------


namespace eye { namespace tracker { namespace message {

//---------------------------------------------------------------------------

bool
parse(std::string const& str, Message& msg)
{
  // Exceptions:
  //  std::domain_error if a JSON value is not an object of the correct type.
  //  std::out_of_range if a specified key is not stored in j.
  try
  {
    msg.json = nlohmann::json::parse(str);
    std::string cat = msg.json.at("category");
    msg.category = eye::tracker::category(cat);
    unsigned stat = msg.json.at("statuscode");
    msg.status = eye::tracker::status(stat);
  }
  catch (std::exception& e)
  {
    eye::debug::error(__FILE__, __LINE__, "exception: ", e.what());
    //std::cout << json.dump(2) << std::endl;
    return false;
  }
  try
  {
    std::string req = msg.json.at("request");
    msg.request = eye::tracker::request(req);
  }
  catch (...) {} // ignore
  try
  {
    msg.values = msg.json.at("values");
  }
  catch (...) {} // ignore
  return true;
}

//---------------------------------------------------------------------------

bool
parse(Message const& msg, Calibration& cal)
{
  using json = nlohmann::json;
  if (!msg.values.count(Message::Value::calibration_result))
  {
    return false;   // calibration result not found
  }
  try
  {
    eye::Calibration c{};

    json j_c = msg.values.at(Message::Value::calibration_result);

    bool result    = j_c.at("result");
    c.success      = result;
    c.error_deg    = eyes(j_c, {"deg", "degl", "degr"});
    c.error_rating = calib_rating(result, c.error_deg);

    json j_points  = j_c.at("calibpoints");
    if (j_points.is_array())
    {
      for (auto const& j_p : j_points)
      {
        eye::Calibration::Point p{};
        unsigned n        = j_p.at("state");
        p.sample_state    = eye::calib_sample_state(n);
        p.calibrate_px    = xy<unsigned>(j_p.at("cp"));
        p.estimated_px    = xy<float>(j_p.at("mecp"));
        p.accuracy_deg    = eyes(j_p.at("acd"), {"ad", "adl", "adr"});
        p.accuracy_rating = calib_rating(n, p.accuracy_deg);
        p.mean_error_px   = eyes(j_p.at("mepix"), {"mep", "mepl", "mepr"});
        p.avg_std_dev_px  = eyes(j_p.at("asdp"), {"asd", "asdl", "asdr"});
        c.points.push_back(p);
      }
      // Success if calibration points were parsed
      if (c.points.size())
      {
        cal = c;
        return true;
      }
    }
  }
  catch (std::exception& e)
  {
    eye::debug::error(__FILE__, __LINE__, "exception: ", e.what());
    //std::cout << json.dump(2) << std::endl;
  }
  return false;
}

//---------------------------------------------------------------------------

// Assumptions:
//  "category": "tracker"
//  "statuscode": 200
bool
parse(Message const& msg, Gaze& gaze)
{
  if (!msg.values.count(Message::Value::gaze_data))
  {
    return false;   // gaze data not found
  }
  // Exceptions:
  //  std::domain_error if a JSON value is not an object of the correct type.
  //  std::out_of_range if a specified key is not stored in j.
  try
  {
    nlohmann::json j = msg.values.at(Message::Value::gaze_data);
    Gaze g{};
    g.timestamp   = j.at("timestamp");
    g.time_ms     = j.at("time");
    g.fixation    = j.at("fix");
    g.tracking    = static_cast<unsigned>(j.at("state"));
    g.raw_px      = xy<float>(j.at("raw"));
    g.avg_px      = xy<float>(j.at("avg"));
    g.pupil_left  = pupil(j.at("lefteye"));
    g.pupil_right = pupil(j.at("righteye"));
    gaze = g;
  }
  catch (std::exception& e)
  {
    eye::debug::error(__FILE__, __LINE__, "exception: ", e.what());
    //std::cout << json.dump(2) << std::endl;
    return false;
  }
  return true;
}

//---------------------------------------------------------------------------

bool
try_update(Message const& msg, Screen& scr)
{
  using v = eye::tracker::Message::Value;

  Screen tmp = scr;

  scr.index = msg.values.value(v::screen_index,     scr.index);
  scr.w_px  = msg.values.value(v::screen_width_px,  scr.w_px);
  scr.h_px  = msg.values.value(v::screen_height_px, scr.h_px);
  scr.w_m   = msg.values.value(v::screen_width_m,   scr.w_m);
  scr.h_m   = msg.values.value(v::screen_height_m,  scr.h_m);

  return (scr != tmp);
}

//---------------------------------------------------------------------------

bool
try_update(Message const& msg, Tracker::State& ts)
{
  using v = eye::tracker::Message::Value;

  Tracker::State tmp = ts;

  auto d = to_number(ts.device_state);
  d = msg.values.value(v::device_state, d);
  ts.device_state = eye::tracker_device_state(d);

  ts.frame_rate     = msg.values.value(v::frame_rate,     ts.frame_rate);
  ts.is_calibrated  = msg.values.value(v::is_calibrated,  ts.is_calibrated);
  ts.is_calibrating = msg.values.value(v::is_calibrating, ts.is_calibrating);

  return (ts != tmp);
}

//---------------------------------------------------------------------------

} } } // eye::tracker::message
//===========================================================================//
