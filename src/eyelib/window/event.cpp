//===========================================================================//
//  Nathan Lucas
//  2015-2016
//===========================================================================//

#include "window/event.hpp"

#include <FL/Fl.H>            // FLTK GUI libraries
#include <FL/names.h>         // fl_eventnames[]
#include <FL/Enumerations.H>

#include <algorithm>    // std::max

namespace {   //-------------------------------------------------------------

using Mouse = eye::window::Event::Mouse;
using Key   = eye::window::Event::Key;
using State = eye::window::Event::State;

} // anonymous --------------------------------------------------------------

namespace eye { namespace window {

/* @brief  Event state.

  Bitfield of what shift states were on and what
  buttons were held down during the most recent event.
  ```
  FL_SHIFT        0x00010000        // One of the shift keys is down
  FL_CAPS_LOCK    0x00020000        // The caps lock is on
  FL_CTRL         0x00040000        // One of the ctrl keys is down
  FL_ALT          0x00080000        // One of the alt keys is down
  FL_NUM_LOCK     0x00100000        // The num lock is on
  FL_META         0x00400000        // One of the meta/Windows keys is down
  FL_SCROLL_LOCK  0x00800000        // The scroll lock is on
  FL_BUTTON1      0x01000000        // Mouse button 1 is pushed
  FL_BUTTON2      0x02000000        // Mouse button 2 is pushed
  FL_BUTTON3      0x04000000        // Mouse button 3 is pushed
  FL_BUTTONS      0x7f000000        // Any mouse button is pushed
  FL_BUTTON(n)   (0x00800000<<(n))  // Mouse button n (n > 0) is pushed
  FL_KEY_MASK     0x0000ffff        // All keys are 16 bit for now
  FL_COMMAND_FL_CTRL  // Alias for FL_CTRL (WIN32|X11) or FL_META (MacOS X)
  FL_CONTROL_FL_META  // Alias for FL_META (WIN32|X11) or FL_CTRL (MacOS X)
  ```
*/


/////////////////////////////////////////////////////////////////////////////
//  Event::Key
/////////////////////////////////////////////////////////////////////////////

Event::Key::Key(int event_code)
: event_code(event_code)
, key_code(Fl::event_key())
{}

bool
Event::Key::is_press() const
{
  return ((event_code == FL_SHORTCUT) ||   // key press, widget unfocused
          (event_code == FL_KEYDOWN));     // key press, widget focused
}

bool
Event::Key::is_release() const
{
  return (event_code == FL_KEYUP);
}

//-----------------------------------------------------------

bool
Key::is_alt() const
{
  return ((key_code == FL_Alt_L) ||
          (key_code == FL_Alt_R));
}

bool
Key::is_control() const
{
  return ((key_code == FL_Control_L) ||
          (key_code == FL_Control_R));
}

bool
Event::Key::is_enter() const
{
  return ((key_code == FL_Enter) ||
          (key_code == FL_KP_Enter));
}

bool
Key::is_function() const
{
  return ((key_code >= FL_F) &&
          (key_code <= FL_F_Last));
}

bool
Key::is_meta() const
{
  return ((key_code == FL_Meta_L) ||
          (key_code == FL_Meta_R));
}

bool
Key::is_numpad() const
{
  return ((key_code >= FL_KP) &&
          (key_code <= FL_KP_Last));
}

bool
Key::is_printable() const
{
  return ((key_code >= 0x20) &&
          (key_code <= 0x7E));
}

bool
Key::is_shift() const
{
  return ((key_code == FL_Shift_L) ||
          (key_code == FL_Shift_R));
}

bool
Key::is_special() const
{
  return special() != Special::unrecognized;
}

//-----------------------------------------------------------

unsigned
Key::function() const
{
  if (is_function())
  {
    return key_code - FL_F;
  }
  return 0;
}

Key::Special
Key::special() const
{
  switch (key_code)
  {
    case FL_Button:       return Special::button;
    case FL_BackSpace:    return Special::backspace;
    case FL_Tab:          return Special::tab;
    case FL_Enter:        return Special::enter;
    case FL_Pause:        return Special::pause;
    case FL_Scroll_Lock:  return Special::scroll_lock;
    case FL_Escape:       return Special::escape;
    case FL_Home:         return Special::home;
    case FL_Left:         return Special::left;
    case FL_Up:           return Special::up;
    case FL_Right:        return Special::right;
    case FL_Down:         return Special::down;
    case FL_Page_Up:      return Special::page_up;
    case FL_Page_Down:    return Special::page_down;
    case FL_End:          return Special::end;
    case FL_Print:        return Special::print;
    case FL_Insert:       return Special::insert;
    case FL_Menu:         return Special::menu;
    case FL_Num_Lock:     return Special::num_lock;
    case FL_KP_Enter:     return Special::keypad_enter;
    case FL_Shift_L:      return Special::shift_left;
    case FL_Shift_R:      return Special::shift_right;
    case FL_Control_L:    return Special::control_left;
    case FL_Control_R:    return Special::control_right;
    case FL_Caps_Lock:    return Special::caps_lock;
    case FL_Meta_L:       return Special::meta_left;
    case FL_Meta_R:       return Special::meta_right;
    case FL_Alt_L:        return Special::alt_left;
    case FL_Alt_R:        return Special::alt_right;
    case FL_Delete:       return Special::delete_key;
    default:              return Special::unrecognized;
  }
}

std::string
Key::to_string() const
{
  if (is_function())  { return "F" + std::to_string(function()); }
  if (is_printable()) { return Fl::event_text(); }

  using S = Event::Key::Special;
  auto s = special();

  if (is_numpad())
  {
    if (s == S::unrecognized)
    {
      return Fl::event_text();
    }
  }
  switch (s)
  {
    case S::unrecognized:   return "Unrecognized";
    case S::button:         return "Button";
    case S::backspace:      return "Backspace";
    case S::tab:            return "Tab";
    case S::enter:          return "Enter";
    case S::pause:          return "Pause";
    case S::scroll_lock:    return "Scroll Lock";
    case S::escape:         return "Escape";
    case S::home:           return "Home";
    case S::left:           return "Left";
    case S::up:             return "Up";
    case S::right:          return "Right";
    case S::down:           return "Down";
    case S::page_up:        return "Page Up";
    case S::page_down:      return "Page Down";
    case S::end:            return "End";
    case S::print:          return "Print Screen";
    case S::insert:         return "Insert";
    case S::menu:           return "Menu";
    case S::num_lock:       return "Num Lock";
    case S::keypad:         return "Keypad";
    case S::keypad_enter:   return "Enter";
    case S::function:       return "Function";
    case S::shift_left:     return "Shift";
    case S::shift_right:    return "Shift";
    case S::control_left:   return "Control";
    case S::control_right:  return "Control";
    case S::caps_lock:      return "Caps Lock";
    case S::meta_left:      return "Meta/Windows";
    case S::meta_right:     return "Meta/Windows";
    case S::alt_left:       return "Alt";
    case S::alt_right:      return "Alt";
    case S::delete_key:     return "Delete";
    default:                return "Error";
  }
}


/////////////////////////////////////////////////////////////////////////////
//  Event::Mouse
/////////////////////////////////////////////////////////////////////////////

Mouse::Mouse(int event_code) : event_code(event_code) {}

bool Mouse::is_click()   const { return (event_code == FL_PUSH); }
bool Mouse::is_release() const { return (event_code == FL_RELEASE); }
bool Mouse::is_drag()    const { return (event_code == FL_DRAG); }
bool Mouse::is_move()    const { return (event_code == FL_MOVE); }
bool Mouse::is_scroll()  const { return (event_code == FL_MOUSEWHEEL); }

bool Mouse::is_button_left()  { return Fl::event_button() == FL_LEFT_MOUSE; }
bool Mouse::is_button_middle(){ return Fl::event_button() == FL_MIDDLE_MOUSE; }
bool Mouse::is_button_right() { return Fl::event_button() == FL_RIGHT_MOUSE; }

Mouse::Button
Mouse::button()
{
  switch (Fl::event_button())
  {
    case FL_LEFT_MOUSE:   return Mouse::Button::left;
    case FL_MIDDLE_MOUSE: return Mouse::Button::middle;
    case FL_RIGHT_MOUSE:  return Mouse::Button::right;
    default:              return Mouse::Button::none;
  }
}

unsigned
Mouse::clicks()
{
  return (unsigned)std::max(Fl::event_clicks(), 0);
}

/////////////////////////////////////////////////////////////////////////////
//  Event::State
/////////////////////////////////////////////////////////////////////////////

// key event ------------------------------------------------

bool State::Key::is_key()                   { return Fl::event_key(); }
int  State::Key::key_code()                 { return Fl::event_key(); }
bool State::Key::is_press(int key_code)     { return Fl::event_key(key_code); }
bool State::Key::is_press_now(int key_code) { return Fl::get_key(key_code); }

// modifier keys --------------------------------------------

bool State::Key::alt()    { return Fl::event_alt(); }
bool State::Key::ctrl()   { return Fl::event_ctrl(); }
bool State::Key::shift()  { return Fl::event_shift(); }

// lock keys ------------------------------------------------

bool State::Key::caps_lock()   { return (Fl::event_state() & FL_CAPS_LOCK); }
bool State::Key::num_lock()    { return (Fl::event_state() & FL_NUM_LOCK); }
bool State::Key::scroll_lock() { return (Fl::event_state() & FL_SCROLL_LOCK); }

// mouse buttons --------------------------------------------

bool State::Mouse::button_left()    { return Fl::event_button1(); }
bool State::Mouse::button_middle()  { return Fl::event_button2(); }
bool State::Mouse::button_right()   { return Fl::event_button3(); }

// mouse position -------------------------------------------

int State::Mouse::x_screen()  { return Fl::event_x_root(); }
int State::Mouse::y_screen()  { return Fl::event_y_root(); }
int State::Mouse::x_window()  { return Fl::event_x(); }
int State::Mouse::y_window()  { return Fl::event_y(); }

bool
State::Mouse::is_inside(int x, int y, int w, int h)
{
  return Fl::event_inside(x, y, w, h);
}

//bool
//State::Mouse::is_inside(Fl_Widget const* w)
//{
//  return Fl::event_inside(w);
//}

// mouse scroll ---------------------------------------------

int State::Mouse::scroll_dx() { return Fl::event_dx(); }
int State::Mouse::scroll_dy() { return Fl::event_dy(); }

//-----------------------------------------------------------

std::string
State::text()
{
  std::string et = Fl::event_text();
  return ((et == "\r") ? "ENTER" : et);
}

unsigned
State::last_event()
{
  return std::max(Fl::event(), 0);
}


/////////////////////////////////////////////////////////////////////////////
//  Event
/////////////////////////////////////////////////////////////////////////////

Event::Event(unsigned time_ms, int code)
: code(code)
, name(fl_eventnames[code])
, time_ms(time_ms)
{}

bool
Event::is_focus() const
{
  return (code == FL_FOCUS) || (code == FL_UNFOCUS);
}

bool
Event::is_key() const
{
  return key.is_press() ||
         key.is_release();
}

bool
Event::is_mouse() const
{
  return mouse.is_click() || mouse.is_release() ||
         mouse.is_drag()  || mouse.is_move()    || mouse.is_scroll();
}


/////////////////////////////////////////////////////////////////////////////

} } // eye::window
//===========================================================================//
