//=============================================================================
/// @file
/// @brief    Window text.
/// @author   Nathan Lucas
/// @date     2016
//=============================================================================
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
