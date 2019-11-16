//===========================================================================//
/// @file
/// @brief    @ref eye::TargetSequence class.
/// @author   Nathan Lucas
/// @date     2015-2016
//===========================================================================//
#ifndef EYE_TARGET_SEQUENCE_HPP
#define EYE_TARGET_SEQUENCE_HPP

#include "eye/target_array.hpp"   // eye::TargetArray, eye::Target

#include <vector>   // std::vector

namespace eye {

/// @addtogroup module_target
/// @{

/** @brief  ...
  @headerfile eye/target_sequence.hpp
  @par Header
    @header{eye/target_sequence.hpp}
    @sa [Reference Manual]( @ref target_sequence)
*/
class TargetSequence
{
public:

  /** @brief  Constructs a target sequence.
  @param  [in]  w_px  Screen width in pixels.
  @param  [in]  h_px  Screen height in pixels.
  @param  [in]  x     Number of targets horizontally.
  @param  [in]  y     Number of targets vertically.

  A target sequence cycles once through an @em x by @em y
  array of evenly spaced targets in randomized order.
  */
  TargetSequence(unsigned w_px, unsigned h_px, unsigned x, unsigned y);

  //-----------------------------------------------------------
  /// @name Process
  /// @{

  /// @brief  Starts the target sequence.
  /// @return `true` if successful, `false` otherwise.
  ///
  /// Returns `false` if the sequence was already started prior to the call.
  bool
  start();

  /// @brief  Aborts target sequence and re-initializes variables.
  /// @return `true` if successful, `false` otherwise.
  ///
  /// Returns `false` if the sequence was not started prior to the call.
  bool
  cancel();

  /// @brief  Returns the target sequence state.
  /// @return `true` if the target sequence has started, `false` otherwise.
  bool
  is_started() const;

  /// @}
  //-----------------------------------------------------------
  /// @name Points
  /// @{

  /// @brief  Returns the current target point.
  /// @return Structure containing target information.
  ///
  Target
  current_point() const;

  /// @brief  Advances to the next target in the sequence.
  /// @return `true` if sequence advanced, `false` if sequence has completed.
  bool
  advance_point();

  /// @brief  Returns all target points.
  /// @return Vector containing all targets.
  ///
  std::vector<Target>
  points() const;

  /// @}
  //-----------------------------------------------------------
  /// @name Sampling
  /// @{

  /// @brief  Start sampling for the current target point.
  /// @return `true` if successful, `false` otherwise.
  ///
  /// Returns `false` if the sequence has not been
  /// started, or if already sampling the current target.
  bool
  start_sampling();

  /// @brief  Stop sampling for the current target point.
  /// @return `true` if successful, `false` otherwise.
  ///
  /// Returns `false` if not sampling the current target prior to the call.
  bool
  stop_sampling();

  /// @brief  Returns the sampling state of the current target point.
  /// @return `true` if sampling is being performed, `false` otherwise.
  bool
  is_sampling() const;

  /// @brief  Returns the sampling completion state of the current point.
  /// @return `true` if sampling has been completed, `false` otherwise.
  bool
  is_sampled() const;

  /// @}

private:

  eye::TargetArray targets_;    // 2D array of fixation targets

  bool  started_;     // true if target sequence is started
  bool  sampled_;     // true if current target was sampled

};

//---------------------------------------------------------------------------//

TargetSequence::TargetSequence(unsigned w_px, unsigned h_px,
                               unsigned x, unsigned y)
: targets_(w_px, h_px, x, y)
, started_(false)
, sampled_(false)
{}

// Process ------------------------------------------------------------------

bool
TargetSequence::start()
{
  if (!started_)
  {
    started_ = true;
    return true;
  }
  return false;
}

bool
TargetSequence::cancel()
{
  if (started_)
  {
    targets_.reset();
    started_  = false;
    sampled_ = false;
    return true;
  }
  return false;
}

bool
TargetSequence::is_started() const
{
  return started_;
}

// Points -------------------------------------------------------------------

Target
TargetSequence::current_point() const
{
  return targets_.current();
}

bool
TargetSequence::advance_point()
{
  sampled_ = false;
  return targets_.advance();
}

std::vector<Target>
TargetSequence::points() const
{
  return targets_.targets();
}


// Sampling -----------------------------------------------------------------

bool
TargetSequence::start_sampling()
{
  if (started_ && !targets_.is_active())
  {
    targets_.is_active(true);
    return true;
  }
  return false;
}

bool
TargetSequence::stop_sampling()
{
  if (targets_.is_active())
  {
    targets_.is_active(false);
    sampled_ = true;
    return true;
  }
  return false;
}

bool
Calibrator::is_sampled() const
{
  return sampled_;
}

/// @}

} // eye

#endif // EYE_TARGET_SEQUENCE_HPP
//===========================================================================//
