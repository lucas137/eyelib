//===========================================================================//
/// @file
/// @brief    Gaze data logger application.
/// @author   Nathan Lucas
/// @date     2016
//===========================================================================//
#ifndef EYE_DATALOG_HPP
#define EYE_DATALOG_HPP

#include <eyelib.hpp>       // eye::Gaze

#include <utl/file.hpp>     // utl::file::file_writer

namespace eye {
/// @addtogroup eyelib_datalog
/// @{

/// @brief  Logs gaze data from the eye tracker to a CSV file.
///
class DataLog
{
public:

  /// @brief  Construct a log file for recording gaze data.
  /// @param  [in]  app_name  Application name.
  explicit                              // direct initialization only
  DataLog(std::string const& app_name);

  ~DataLog() = default;                         ///< Destructor.
  DataLog(DataLog const&)            = delete;  ///< Prohibit copying.
  DataLog& operator=(DataLog const&) = delete;  ///< Prohibit assignment.

  /// @brief  Connect to an eye gaze tracker server and run the application.
  /// @param  [in]  scr   Screen index.
  /// @return Exit code.
  int run(unsigned scr);

private:

  // Callback to record gaze data
  void write(eye::Gaze const& g);

  utl::file::file_writer  log_file_;    // Log file writer
};

/// @}
} // eye

#endif // EYE_DATALOG_HPP
//===========================================================================//
