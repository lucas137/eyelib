//=============================================================================
/// @file
/// @brief    FLTK window event.
/// @author   Nathan Lucas
/// @date     2016
//=============================================================================
#ifndef EYLIB_WINDOW_EVENT_HPP
#define EYLIB_WINDOW_EVENT_HPP

#include <string>     // std::string

namespace eye { namespace window {

/// @ingroup    window
/// @addtogroup event  Window Events
/// @{

//---------------------------------------------------------------------------
/// Event data.
struct Event
{
  // Event handler return values for FLTK
  static constexpr int HANDLED     = 1;    ///< Event was handled.
  static constexpr int NOT_HANDLED = 0;    ///< Event was not handled.

  //-----------------------------------------------------------
  /// Key event.
  struct Key
  {
    /// Non-printable keys.
    enum class Special
    {
      unrecognized  = 0x0000, ///< Unrecognized event key.
      button        = 0xfee8, ///< Mouse button; use Fl_Button + n for button n.
      backspace     = 0xff08, ///< Backspace key.
      tab           = 0xff09, ///< Tab key.
      enter         = 0xff0d, ///< Enter key.
      pause         = 0xff13, ///< Pause key.
      scroll_lock   = 0xff14, ///< Scroll lock key.
      escape        = 0xff1b, ///< Escape key.
      home          = 0xff50, ///< Home key.
      left          = 0xff51, ///< Left arrow key.
      up            = 0xff52, ///< Up arrow key.
      right         = 0xff53, ///< Right arrow key.
      down          = 0xff54, ///< Down arrow key.
      page_up       = 0xff55, ///< Page-up key.
      page_down     = 0xff56, ///< Page-down key.
      end           = 0xff57, ///< End key.
      print         = 0xff61, ///< Print (or print-screen) key.
      insert        = 0xff63, ///< Insert key.
      menu          = 0xff67, ///< Menu key.
      num_lock      = 0xff7f, ///< Num lock key.
      keypad        = 0xff80, ///< Keypad number; use FL_KP + 'n' for digit n.
      keypad_enter  = 0xff8d, ///< Enter key on the keypad, same as Fl_KP+'\\r'.
      keypad_last   = 0xffbd, ///< Last keypad key; use to range-check keypad.
      function      = 0xffbd, ///< Function key; use FL_F + n for function key n.
      function_last = 0xffe0, ///< Last function key; use to range-check keys.
      shift_left    = 0xffe1, ///< Lefthand shift key.
      shift_right   = 0xffe2, ///< Righthand shift key.
      control_left  = 0xffe3, ///< Lefthand control key.
      control_right = 0xffe4, ///< Righthand control key.
      caps_lock     = 0xffe5, ///< Caps lock key.
      meta_left     = 0xffe7, ///< Left meta/Windows key.
      meta_right    = 0xffe8, ///< Right meta/Windows key.
      alt_left      = 0xffe9, ///< Left alt key.
      alt_right     = 0xffea, ///< Right alt key.
      delete_key    = 0xffff, ///< Delete key.
    };

    int event_code{0};        ///< Event code.
    int key_code{0};          ///< Valid for key events only.

    Key(int event_code);      ///< Constructor.

    // Member functions ------

    /// @name Key Press and Release
    /// @{
    bool is_press() const;    ///< `true` if key was pressed.
    bool is_release() const;  ///< `true` if key was released.
    /// @}
    /// @name Inspection.
    /// @{
    bool is_alt() const;        ///< 'true' if left or right alt key.
    bool is_control() const;    ///< 'true' if left or right control key.
    bool is_enter() const;      ///< `true` if enter key.
    bool is_function() const;   ///< `true` if numbered function key.
    bool is_meta() const;       ///< 'true' if left or right meta key.
    bool is_numpad() const;     ///< `true` if numeric keypad key.
    bool is_printable() const;  ///< `true` if printable ASCII character.
    bool is_shift() const;      ///< 'true' if left or right shift key.
    bool is_special() const;    ///< `true` if printable ASCII character.
    /// @}
    /// @name Get key.
    /// @{

    /// Return function key number, or `0` if event was not a function key.
    unsigned function() const;

    /// Return non-printable key, or Special::unrecognized.
    Special special() const;

    /// Return a string representation a key.
    std::string to_string() const;

    /// @}
  };

  //-----------------------------------------------------------
  struct Mouse
  {
    /// Mouse button.
    enum class Button : unsigned
    {
      none   = 0,
      left   = 1,
      middle = 2,
      right  = 3
    };

    int event_code{0};      ///< Event code.

    Mouse(int event_code);  ///< Constructor.

    // Member functions ------

    /// @name Mouse Button
    /// @{
    bool        is_click()   const; ///< `true` if button was clicked.
    bool        is_release() const; ///< `true` if button was released.
    static bool is_button_left();   ///< `true` if left button click/release.
    static bool is_button_middle(); ///< `true` if middle button click/release.
    static bool is_button_right();  ///< `true` if right button click/release.
    static Button   button();       ///< Button that was clicked or released.
    static unsigned clicks();       ///< Return N-1 for N clicks.
    /// @}
    /// @name Mouse Position
    /// @{
    bool is_drag() const;     ///< `true` if moved with a button held down.
    bool is_move() const;     ///< `true` if moved without a button held down.
    /// @}
    /// @name Mouse Scroll Wheel
    /// @{
    bool is_scroll() const;   ///< `true` if mouse scroll wheel moved.
    /// @}
    //-----------------------------------------------------------
    //  /// @name Mouse Events
    //  /// @{
    //  static constexpr int Button = 0xfee8;  ///< Mouse button; use `Button` + n.
    //  /// @}
    //-----------------------------------------------------------
  };

  //-----------------------------------------------------------
  /// Key and mouse states during most recent event.
  struct State
  {
    /// Key event states.
    struct Key
    {
      /// @name Key event.
      /// @{
      static bool is_key();   ///< `true` if last event was key press or release.
      static int  key_code(); ///< Key code of last event, or `0` if no key.
      static bool is_press(int Key);    /// `true` if pressed during last event.
      static bool is_press_now(int Key);  /// `true` if key is held down now.
      /// @}
      /// @name Modifier keys.
      /// @{
      static bool alt();    ///< `true` if Alt key is pressed.
      static bool ctrl();   ///< `true` if Ctrl key is pressed.
      static bool shift();  ///< `true` if Shift key is pressed.
      /// @}
      /// @name Lock keys.
      /// @{
      static bool caps_lock();    ///< `true` if Caps Lock is on.
      static bool num_lock();     ///< `true` if Num Lock is on.
      static bool scroll_lock();  ///< `true` if Scroll Lock is on.
      /// @}
    };

    /// Mouse event states.
    struct Mouse
    {
      /// @name Mouse buttons.
      /// @{
      static bool button_left();   ///< `true` if left button click or release.
      static bool button_middle(); ///< `true` if middle button click or release.
      static bool button_right();  ///< `true` if right button click or release.
      /// @}
      /// @name Mouse position.
      /// @{
      static int x_screen();    ///< Horizontal position on the screen.
      static int y_screen();    ///< Vertical position on the screen.
      static int x_window();    ///< Horizontal position relative to window.
      static int y_window();    ///< Vertical position relative to window.

      /// `true` if mouse event is inside a given rectangle.
      static bool is_inside(int x, int y, int w, int h);

      //  /// `true` if mouse event is inside a given child widget.
      //  static bool is_inside(Fl_Widget const* w);
      /// @}
      /// @name Mouse scroll.
      /// @{
      static int scroll_dx();   ///< Horizontal scroll -- right is positive.
      static int scroll_dy();   ///< Vertical scroll -- down is positive.
      /// @}
    };

    static std::string  text();         ///< Text assocatied with event.
    static unsigned     last_event();   ///< Return last event processed.
  };

  //-----------------------------------------------------------

  // Member variables
  int         code{0};      ///< Event code.
  Key         key{code};    ///< Key event.
  Mouse       mouse{code};  ///< Mouse event.
  std::string name{};       ///< Event name.
  State       state{};      ///< Current event states.
  unsigned    time_ms{0};   ///< Timestamp in milliseconds.

  // Constructors
  Event() = default;                  ///< Default construct event object.
  Event(unsigned time_ms, int code);  ///< Construct event object.

  //-----------------------------------------------------------

  bool is_focus() const;    ///< `true` if focus/unfocus event.
  bool is_key() const;      ///< `true` if key was pressed or released.
  bool is_mouse() const;    ///< `true` if mouse event occurred.
};

//---------------------------------------------------------------------------

/// @}

} } // eye::window

#endif // EYLIB_WINDOW_EVENT_HPP
//===========================================================================//
