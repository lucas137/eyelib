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
/// @brief    Window text.
/// @author   Nathan Lucas
//===========================================================================//
#ifndef EYLIB_WINDOW_TEXT_HPP
#define EYLIB_WINDOW_TEXT_HPP

#include <array>      // std::array
#include <string>     // std::string

namespace eye { namespace window {

/// @ingroup    window
/// @addtogroup text  Window Text
/// @{

//---------------------------------------------------------------------------

struct TextLine       ///< Single line of text.
{
  std::string str{};  ///< String containing text to draw.
  int x_px{0};        ///< Horizontal position in pixels.
  int y_px{0};        ///< Vertical position of text in pixels.
  TextLine(std::string const& str, int x_px, int y_px);
  TextLine() = default;
};

//---------------------------------------------------------------------------

struct TextWidget     ///< Three lines of text.
{
  bool show{true};    ///< `true` to draw on screen.

  /** @brief  Construct text lines.

    Example:
    ```
    Text t(scr.w_px/2, scr.h_px/3, "Eye Tracker Application");`
    ```
  */
  TextWidget(int x_px, int y_px, std::string const& intro);

  TextWidget() = default;   ///< Default constructor.
  void draw() const;        ///< Draw lines of text.

private:
  static constexpr std::size_t size_ = 5;
  int cx_{0};     // center of screen
  std::array<TextLine, size_> lines_{};
};

//---------------------------------------------------------------------------

/// @}

} } // eye::window

#endif // EYLIB_WINDOW_TEXT_HPP
//===========================================================================//
