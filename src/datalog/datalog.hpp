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
/// @brief    Gaze data logger application.
/// @author   Nathan Lucas
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
