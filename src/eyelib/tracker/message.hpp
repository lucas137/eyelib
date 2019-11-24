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
/// @brief    Eye tracker message.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYELIB_TRACKER_MESSAGE_HPP
#define EYELIB_TRACKER_MESSAGE_HPP
/*-----------------------------------------------------------------------------

  Message  |Category     | Request   |Status| Values
  ---------|-------------|-----------|------|----------------------------------
  Request  |`tracker`    |`get`      | N/A  | Array: Attribute keys
  Response |`tracker`    |`get`      |`200` | Object: Attribute key-value(s)
  Request  |`tracker`    |`set`      | N/A  | Object: Attribute key-value(s)
  Response |`tracker`    |`set`      |`200` | N/A
  Request  |`calibration`|`start`    | N/A  | `pointcount` key-value object
  Response |`calibration`|`start`    |`200` | N/A
  Request  |`calibration`|`pointstart`| N/A | Array of `x` and `y` key-values
  Response |`calibration`|`pointstart`|`200`| N/A
  Request  |`calibration`|`pointend` | N/A  | N/A
  Response |`calibration`|`pointend` |`200` | N/A except last point
  Response |`calibration`|`pointend` |`200` | Object: `calibresult` (last point)
  Request  |`calibration`|`abort`    | N/A  | N/A
  Response |`calibration`|`abort`    |`200` | N/A
  Request  |`calibration`|`clear`    | N/A  | N/A
  Response |`calibration`|`clear`    |`200` | N/A
  Request  |`heartbeat`  | N/A       | N/A  | N/A
  Response |`heartbeat`  | N/A       |`200` | N/A
  Response | *           | *         |`400` | Object: Client error key-value(s)
  Response | *           | *         |`500` | Object: Server error key-value(s)

  Note: * = Any category and request.

  Status Code | Description
  ------------|--------------------
    `200`     | OK
    `400`     | Client error
    `500`     | Server error
    `800`     | Calibration change
    `801`     | Screen state change
    `802`     | Device state change

  Attribute          | Type        | get | set | Remark
  -------------------|-------------|-----|-----|-----------------------------
  `push`             | `bool`      | X   | X   |
  `heartbeatinterval`| `unsigned`  | X   |     | Immutable
  `version`          | `unsigned`  | X   | X   |
  `trackerstate`     | `unsigned`  | X   |     | Immutable
  `framerate`        | `unsigned`  | X   |     | Immutable
  `iscalibrated`     | `bool`      | X   |     | Immutable
  `iscalibrating`    | `bool`      | X   |     | Immutable
  `cailbresult`      | JSON object | X   |     | Immutable; Use for pull mode
  `frame`            | JSON object | X   |     | Immutable; Use for pull mode
  `screenindex`      | `unsigned`  | X   | X   |
  `screenresw`       | `unsigned`  | X   | X   |
  `screenresh`       | `unsigned`  | X   | X   |
  `screenpsyw`       | `float`     | X   | X   |
  `screenpsyh`       | `float`     | X   | X   |
  `statusmessage`    | `string`    |     | X   |

  Note: Immutable attributes are defined by the tracker server.

  Frame object | Type        | Remark
  -------------|-------------|-------------------------------
  `timestamp`  | `string`    | e.g. `2014-04-15 15:28:46.628`
  `time`       | `int`       | Timestamp in milliseconds
  `fix`        | `bool`      | Is fixated?
  `state`      | `int`       | 32bit masked state integer
  `raw`        | JSON object | `{"x":int,"y":int}`
  `avg`        | JSON object | `{"x":int,"y":int}`
  `lefteye`    | JSON object | See eye object table
  `rightteye`  | JSON object | See eye object table

  Eye object | Type        | Remark
  -----------|-------------|-------------------------------
  `raw`      | JSON object | `{"x":int,"y":int}`
  `avg`      | JSON object | `{"x":int,"y":int}`
  `psize`    | `float`     | Pupil size
  `pcenter`  | JSON object | `{"x":float,"y":float}`


  Calibration   | Type        | Remark
  --------------|-------------|-----------------------------------
  `result`      | `bool`      | Was the calibration successful?
  `deg`         | `float`     | Average error in degrees
  `degl`        | `float`     | Average error in degs, left eye
  `degr`        | `float`     | Average error in degs, right eye
  `calibpoints` | JSON array  | See calibpoint object table

  Calibpoint  | Type        | Remark
  ------------|-------------|------------------------------------------
  `state`     | `int`       | State of calibration point
  `cp`        | JSON object | `{"x":float,"y":float}`
  `mecp`      | JSON object | `{"x":float,"y":float}`
  `acd`       | JSON object | `{"ad":float,"adl":float,"adr":float}`
  `mepix`     | JSON object | `{"mep":float,"mepl":float,"mepr":float}`
  `asdp`      | JSON object | `{"asd":float,"asdl":float,"asdr":float}`


  //// Notifications ////

  Calibration change:
  {
    "category": "calibration",
    "statuscode": 800
  }
  Screen parameter change:
  {
    "category": "tracker",
    "statuscode": 801
  }
  Device state change:
  {
    "category": "tracker",
    "statuscode": 802
  }


-------------------------------------------------------------------------------
  Examples:

  // Assemble request message
  auto get_state = msg::get({ Msg::Value::device_state,
                              Msg::Value::frame_rate,
                              Msg::Value::is_calibrated,
                              Msg::Value::is_calibrating });
  tcp_.write(get_state.dump());

  // Pre-defined message
  tcp_.write(msg::GET_TRACKER_STATE);

-------------------------------------------------------------------------------
*/

#include <eyelib.hpp>

//#include <eyelib/calibration.hpp>   // eye::Calibration
//
//#include <eyelib/gaze.hpp>        // eye::Gaze
//#include <eyelib/screen.hpp>      // eye::Screen
//#include <eyelib/tracker.hpp>     // eye::Tracker::State

#include <utl/json.hpp>     // nlohmann::json

#include <ostream>    // std::ostream
#include <string>     // std::string

namespace eye { namespace tracker {

/// @addtogroup eyelib_message
/// @{

/////////////////////////////////////////////////////////////////////////////
//  Message
/////////////////////////////////////////////////////////////////////////////

/// @brief  Message to or from a tracker server.
///
class Message
{
public:
  //-----------------------------------------------------------
  /// @name Attribute Types
  /// @{

  // category    | valid requests
  // ------------|---------------
  // tracker     | get, set
  // calibration | start, point_start, point_end, abort, clear
  // heartbeat   | N/A

  /// Message categories.
  enum class Category
  {
    tracker, calibration, heartbeat, unknown
  };

  /// Message request type.
  enum class Request
  {
    get,
    set,
    start,
    point_start,
    point_end,
    abort,
    clear,
    unknown
  };

  /// Message status code.
  enum class Status : unsigned
  {
    unknown       = 0,    ///< Missing, unrecognized, or invalid status code.
    ok            = 200,  ///< Standard response for successful requests.
    bad_request   = 400,  ///< Failed request due to client error.
    server_error  = 500,  ///< Failed to fulfill an apparently valid request.
    calib_change  = 800,  ///< Notification calibration state changed.
    screen_change = 801,  ///< Notification screen parameters changed.
    device_change = 802,  ///< Notification device connection state changed.
  };

  /// Message value keys.
  struct Value
  {
    static constexpr auto calibration_result = "calibresult";
    static constexpr auto device_state       = "trackerstate";
    static constexpr auto frame_rate         = "framerate";
    static constexpr auto gaze_data          = "frame";
    static constexpr auto heartbeat_interval = "heartbeatinterval";
    static constexpr auto is_calibrated      = "iscalibrated";
    static constexpr auto is_calibrating     = "iscalibrating";
    static constexpr auto push_mode          = "push";
    static constexpr auto screen_index       = "screenindex";
    static constexpr auto screen_width_px    = "screenresw";
    static constexpr auto screen_height_px   = "screenresh";
    static constexpr auto screen_width_m     = "screenpsyw";
    static constexpr auto screen_height_m    = "screenpsyh";
    static constexpr auto status_message     = "statusmessage";
    static constexpr auto version            = "version";
  };

  /// @}
  //-----------------------------------------------------------
  /// @name Data Member
  /// @{

  /// @brief  Deserialized message.
  ///
  /// JSON object for a deserialized message.
  nlohmann::json  json{};

  /// @brief  Message category.
  ///
  /// Required for all messages.
  Category  category{Category::unknown};

  /// @brief  Request type (optional).
  ///
  /// Required for all requests and responses, except heartbeat messages.
  /// Omitted in notification messages received from the server.
  Request   request{Request::unknown};

  /// @brief  HTTP/custom status code.
  ///
  /// Required for all responses.  The tracker server includes an HTTP status
  /// code compliant number indicating request success or error condition.
  /// Custom code values are used in notification messages from the server.
  Status  status{Status::unknown};

  /// @brief  Parameters (optional).
  ///
  /// Optionally contains a JSON array of keys for `get` request, or a JSON
  /// object of key-value pairs for either `set` request or `get` response.
  nlohmann::json  values{};

  /// @}
  //-----------------------------------------------------------
  /// @name Operations
  /// @{

  /// Return `true` if an element in @a values is identified with @a key.
  bool
  has_value(std::string const& key) const
  {
    return values.count(key);
  }

  /// @}
  //-----------------------------------------------------------
};

//---------------------------------------------------------------------------

namespace message {

  //-----------------------------------------------------------
  /// @name Deserialize and Parse
  /// @{
  /*
    Note:  These functions are responsible for deserialization
    and parsing ONLY.  They do not:

    - Automatically respond to notifications.
    - Forward messages or message data to callbacks/subscribers.
  */

  /// @brief  Deserialize and parse string to message.
  /// @param  [in]  str   String containing JSON.
  /// @param  [in]  msg   Message object.
  /// @return `true` if successful.
  bool
  parse(std::string const& str, Message& msg);

  /// @brief  Parse string to calibration object.
  /// @param  [in]  msg   Message object.
  /// @param  [in]  cal   Calibration results.
  /// @return `true` if @a cal was update.
  ///
  /// Updates @a cal if valid results are found in @a msg.  If results
  /// are not found or an error occurs, @a cal remains unchanged.
  bool
  parse(Message const& msg, Calibration& cal);

  bool
  parse(Message const& msg, Gaze& gaze);

  #if 0
  /// ...
  template<typename T>
  static bool
  parse(nlohmann::json const& json, std::string const& key, T& val);
  #endif

  //-----------------------------------------------------------

  /// @brief  Update screen object.
  /// @param  [in]  msg   Message object.
  /// @param  [in]  scr   Screen parameters.
  /// @return `true` if @a scr was update.
  ///
  /// Searches @a msg for elements containing screen parameter values.  Updates
  /// each @a scr value found in @a msg.  Elements not found are unchanged.
  bool
  try_update(Message const& msg, Screen& scr);

  /// @brief  Update tracker state object.
  /// @param  [in]  msg   Message object.
  /// @param  [in]  ts    Tracker state object.
  /// @return `true` if @a ts was update.
  ///
  /// Searches @a msg for elements containing tracker state values.  Updates
  /// each @a ts value found in @a msg.  Elements not found are unchanged.
  bool
  try_update(Message const& msg, Tracker::State& ts);

  /// @}
  //-----------------------------------------------------------

} // message

/////////////////////////////////////////////////////////////////////////////
/// @name Category
/// @{

/// Convert string to message category.
inline Message::Category
category(std::string const& str)
{
  using c = Message::Category;
  return ((str == "tracker")     ? c::tracker :
          (str == "calibration") ? c::calibration :
          (str == "heartbeat")   ? c::heartbeat :
                                   c::unknown);
}

/// Convert to string.
inline std::string
to_string(Message::Category const& val)
{
  using c = Message::Category;
  switch (val)
  {
    case c::tracker:     return "tracker";
    case c::calibration: return "calibration";
    case c::heartbeat:   return "heartbeat";
    case c::unknown:
    default:             return "unknown";
  }
}

/// Insert into output stream.
inline std::ostream&
operator<<(std::ostream& os, Message::Category const& val)
{
  return os << eye::tracker::to_string(val);
}

/// @}
/////////////////////////////////////////////////////////////////////////////
/// @name Request Type
/// @{

/// Convert string to message request type.
inline Message::Request
request(std::string const& str)
{
  using r = Message::Request;
  return (str == "get"        ? r::get :
          str == "set"        ? r::set :
          str == "start"      ? r::start :
          str == "pointstart" ? r::point_start :
          str == "pointend"   ? r::point_end :
          str == "abort"      ? r::abort :
          str == "clear"      ? r::clear :
                                r::unknown);
}

/// Convert to string.
inline std::string
to_string(Message::Request const& val)
{
  using r = Message::Request;
  switch (val)
  {
    case r::get:         return "get";
    case r::set:         return "set";
    case r::start:       return "start";
    case r::point_start: return "pointstart";
    case r::point_end:   return "pointend";
    case r::abort:       return "abort";
    case r::clear:       return "clear";
    case r::unknown:
    default:             return "unknown";
  }
}

/// Insert into output stream.
inline std::ostream&
operator<<(std::ostream& os, Message::Request const& val)
{
  return os << eye::tracker::to_string(val);
}

/// @}
/////////////////////////////////////////////////////////////////////////////
/// @name Status Code
/// @{

/// Convert numeric value to status code.
inline Message::Status
status(unsigned code)
{
  using s = Message::Status;
  return ((code == 200) ? s::ok :
          (code == 400) ? s::bad_request :
          (code == 500) ? s::server_error :
          (code == 800) ? s::calib_change :
          (code == 801) ? s::screen_change :
          (code == 802) ? s::device_change :
                          s::unknown);
}

/// Convert to string.
inline std::string
to_string(Message::Status const& val)
{
  using s = Message::Status;
  switch (val)
  {
    case s::ok:            return "OK";
    case s::bad_request:   return "Bad Request";
    case s::server_error:  return "Server Error";
    case s::calib_change:  return "Calibration Changed";
    case s::screen_change: return "Screen State Changed";
    case s::device_change: return "Device State Changed";
    case s::unknown:
    default:               return "Unrecognized Code";
  }
}

/// Convert to numeric code.
inline unsigned
to_number(Message::Status const& val)
{
  return static_cast<unsigned>(val);
}

/// Insert into output stream.
inline std::ostream&
operator<<(std::ostream& os, Message::Status const& val)
{
  return os << eye::tracker::to_number(val) << ' '
            << eye::tracker::to_string(val);
}

/// @}
/////////////////////////////////////////////////////////////////////////////
//    Pre-defined Messages
/////////////////////////////////////////////////////////////////////////////
namespace message {

/// @name Tracker Requests
/// @{

/// Return message to read specified value.
inline nlohmann::json
get(std::string const& key)
{
  return {{ "category", "tracker" },
          { "request", "get" },
          { "values", { key } }};
}

/// Return message to read specified values.
inline nlohmann::json
get(std::vector<std::string> const& keys)
{
  return {{ "category", "tracker" },
          { "request", "get" },
          { "values", keys }};
}

/// Return message to write specified value.
template<typename T>
inline nlohmann::json
set(std::string const& key, T const& val)
{
  return {{ "category", "tracker" },
          { "request", "set" },
          { "values", {{ key, val }} }};
}

/// Return message to write specified values.
inline nlohmann::json
set(nlohmann::json const& values)
{
  return {{ "category", "tracker" },
          { "request", "set" },
          { "values", values }};
}

/// Return message to write specified values.
inline nlohmann::json
set(Screen const& scr)
{
  return set({{ "screenindex", scr.index },
              { "screenresw", scr.w_px },
              { "screenresh", scr.h_px },
              { "screenpsyw", scr.w_m },
              { "screenpsyh", scr.h_m }});
}

/// @}
//---------------------------------------------------------------------------
/// @name Calibration Requests
/// @{

/// Return message to prepare tracker for calibration.
inline nlohmann::json
calibration_start(unsigned point_count)
{
  return {{ "category", "calibration" },
          { "request", "start" },
          { "values", {
            { "pointcount", point_count }}}};
}

/// Return message to start calibration point.
inline nlohmann::json
calibration_point_start(unsigned x, unsigned y)
{
  return {{ "category", "calibration" },
          { "request", "pointstart" },
          { "values", {
            { "x", x },
            { "y", y }}}};
}

/// Return message to stop calibration point.
inline nlohmann::json
calibration_point_end()
{
  return {{ "category", "calibration" },
          { "request", "pointend" }};
}

/// Return message to cancel ongoing calibration sequence.
inline nlohmann::json
calibration_abort()
{
  return {{ "category", "calibration" },
          { "request", "abort" }};
}

/// Return message to remove current calibration from tracker.
inline nlohmann::json
calibration_clear()
{
  return {{ "category", "calibration" },
          { "request", "clear" }};
}

/// @brief Return message to request latest valid calibration result
/// result object and current calibration state.
inline nlohmann::json
calibration_result()
{
  return {{ "category", "tracker" },
          { "request", "get" },
          { "values",
            { "calibresult", "iscalibrated", "iscalibrating" }}};
}

/// Return message to request current calibration state.
inline nlohmann::json
calibration_state()
{
  return {{ "category", "tracker" },
          { "request", "get" },
          { "values",
            { "iscalibrated", "iscalibrating" }}};
}

/// @}
//---------------------------------------------------------------------------
/// @name Pre-defined Requests
/// @{

/// Request latest valid calibration result and current state.
constexpr auto GET_CALIBRATION = "{"
    "\"category\":\"tracker\",\"request\":\"get\",\"values\":["
      "\"calibresult\""
      //"\"calibresult\","
      //"\"iscalibrated\","
      //"\"iscalibrating\""
    "]}";

/// Request physical device connection state.
constexpr auto GET_DEVICE_STATE = "{"
    "\"category\":\"tracker\",\"request\":\"get\",\"values\":["
      "\"trackerstate\""
    "]}";

/// Request latest gaze data frame.
constexpr auto GET_GAZE_DATA = "{"
    "\"category\":\"tracker\",\"request\":\"get\",\"values\":["
      "\"frame\""
    "]}";

/// Request screen parameters.
constexpr auto GET_SCREEN = "{"
    "\"category\":\"tracker\",\"request\":\"get\",\"values\":["
      "\"screenindex\","
      "\"screenresw\","
      "\"screenresh\","
      "\"screenpsyw\","
      "\"screenpsyh\""
    "]}";

/// Request physical device connection state.
constexpr auto GET_TRACKER_STATE = "{"
    "\"category\":\"tracker\",\"request\":\"get\",\"values\":["
      "\"trackerstate\","
      "\"framerate\","
      "\"iscalibrated\","
      "\"iscalibrating\""
    "]}";

/// Request connection with server.
constexpr auto REQUEST_CONNECT = "{"
    "\"category\":\"tracker\","
    "\"request\":\"set\","
    "\"values\":{"
      "\"push\":true,"
      "\"version\":1"
    "}}";

/// Send heartbeat message to server.
constexpr auto REQUEST_HEARTBEAT = "{"
    "\"category\":\"heartbeat\""
  "}";

/// @}
} // message
/////////////////////////////////////////////////////////////////////////////

/// @}
} } // eye::tracker

#endif // EYELIB_TRACKER_MESSAGE_HPP
//===========================================================================//
