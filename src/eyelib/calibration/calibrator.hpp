//===========================================================================//
/// @file
/// @brief    Eye tracker calibrator.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYELIB_CALIBRATOR_HPP
#define EYELIB_CALIBRATOR_HPP

#include <eyelib/screen.hpp>

#include "gaze/gaze_target.hpp"
#include "tracker/message.hpp"
#include "window/window.hpp"

#include <utl/asio/tcp/client.hpp>  // utl::io::tcp::server

//#include <mutex>      // std::mutex, std::lock_guard

namespace eye { namespace tracker {

/// Eye tracker calibrator.
///
class Calibrator
{
public:

  Calibrator(utl::io::tcp::client& tcp);

  void setup(Window& win, Targets const& points,
             TargetDuration const& target_ms);

  void process_response(Message const& m);

private:
  GazeTarget  gaze_target_{};     // sequence of targets
  utl::io::tcp::client& tcp_;
};

} } // eye::tracker

#endif // EYELIB_CALIBRATOR_HPP
//===========================================================================//
