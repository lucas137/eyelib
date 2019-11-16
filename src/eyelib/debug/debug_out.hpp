//=============================================================================
/// @file
/// @brief    Debug console output.
/// @author   Nathan Lucas
/// @date     2016
//=============================================================================
#ifndef EYELIB_DEBUG_HPP
#define EYELIB_DEBUG_HPP

#include <iostream>   // std::cerr
#include <string>     // std::string

namespace eye { namespace debug {
//---------------------------------------------------------------------------

inline void
error(std::string const& file, unsigned line, std::string const& str)
{
  std::cerr << file << ":" << line
      <<'\n'<< "eyelib: ERROR: " << str <<'\n';
}

template<typename T>
inline void
error(std::string const& file, unsigned line,
      std::string const& str,  T const& val)
{
  std::cerr << file << ":" << line
      <<'\n'<< "eyelib: ERROR: " << str << ":"
      <<'\n'<< val <<'\n';
}

//---------------------------------------------------------------------------
} } // eye::debug

#endif // EYELIB_DEBUG_HPP
//===========================================================================//
