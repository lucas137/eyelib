//===========================================================================//
//  Nathan Lucas
//  2016
//===========================================================================//

#include "window/window.hpp"

#include <eyelib.hpp>

#include "debug/debug_out.hpp"      // eye::debug
#include "window/calib_widget.hpp"  // eye::window::CalibWidget
#include "window/gaze_widget.hpp"   // eye::window::GazeWidget
#include "window/target_widget.hpp" // eye::window::TargetWidget
#include "window/text_widget.hpp"   // eye::window::Text

#include <utl/fltk/fltk_lock.hpp>   // utl::fltk::scoped_lock

#include <FL/Fl.H>          // FLTK GUI libraries
#include <FL/fl_ask.H>      // fl_choice
#include <FL/fl_draw.H>     // fl_color, fl_cursor
#include <FL/Fl_Double_Window.H>

#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Menu_Item.H>

#include <utl/memory.hpp>   // utl::make_unique

#include <iostream>     // std::cout
#include <string>       // std::to_string

namespace {   //-------------------------------------------------------------

using window_lock = utl::fltk::scoped_lock;

// Confirm user intent and close window.
// Invoked by main thread.  Do not call Fl::lock() and Fl::unlock().
void
exit_callback(Fl_Widget*, void*)
{
  // Confirmation popup
  if (fl_choice("Are you sure you want to quit?",
                "No (Esc)", "Yes (Enter)", NULL))
  {
    while (Fl::first_window())      // Hide all windows, ensuring
    {                               // Fl::run() returns and all
      Fl::first_window()->hide();   // Destructors are called
    }
  }
}

} // anonymous --------------------------------------------------------------

namespace eye {


/////////////////////////////////////////////////////////////////////////////
//  Window Implementation
/////////////////////////////////////////////////////////////////////////////

struct Window::Impl
: public Fl_Double_Window
{
  Impl(Tracker& tracker, Screen const& scr,
       std::string const& title, ColorRGB const& bg);
  ~Impl();

  int  run();               // Run until window is closed
  void draw() override;     // Draw the window

  // @brief  Process a window event.
  // @param  [in] event_code   FLTK event code.
  // @return `1` if event was handled, `0` otherwise.
  int  handle(int event_code) override;
  bool handle_key_press(Event const& event);
  void handle_right_click();

  // Eye tracker callbacks
  void handle(Calibration const& c);      // Calibration results
  void handle(Gaze const& g);             // Gaze data handler
  void handle(Tracker::State const& s);   // Tracker state change handler

  // Event callbacks
  static void toggle_raw_gaze(Fl_Widget*, void *userdata);
  static void toggle_avg_gaze(Fl_Widget*, void *userdata);
  static void show_calib_average(Fl_Widget*, void *userdata);
  static void show_calib_points(Fl_Widget*, void *userdata);
  static void show_calib_none(Fl_Widget*, void *userdata);

  //---------------------------------------------------------------
  // Callbacks

  Window::event_handler event_callback_{[](Event const&){}};
  Window::state_handler state_callback_{[](State const&){}};

  // Window::Impl::run() saves tracker_'s current calibration and gaze
  // handlers before registering the associated Window::Impl::handle()
  // callbacks.  The handle() callbacks then invoke the saved handlers.
  // When the window is destroyed, the saved handlers are re-registered
  // with tracker_ to restore the original callbacks.
  Tracker::calib_handler  calib_callback_{[](eye::Calibration const&){}};
  Tracker::gaze_handler   gaze_callback_{[](eye::Gaze const&){}};

  //---------------------------------------------------------------

  Window::State         state_{State::init};  // Window state
  window::CalibWidget   calib_{};             // Calibration results
  window::TargetWidgets targets_{};           // Visual targets
  window::GazeWidget    gaze_{};              // Gaze point
  window::TextWidget    text_{};              // Text overlay
  Tracker&              tracker_;             // Eye tracker
};

//---------------------------------------------------------------------------

Window::Impl::Impl(Tracker& tracker, Screen const& scr,
                   std::string const& title, ColorRGB const& bg)
: Fl_Double_Window(scr.x_px, scr.y_px, scr.w_px, scr.h_px, title.c_str())
, text_(scr.w_px/2, scr.h_px/3, title)
, tracker_(tracker)
{
  // Convert RGB components to single color value, and set window background
  unsigned bg_color = ((bg.r << 24) | (bg.g << 16) | (bg.b << 8));
  color(bg_color);

  callback(exit_callback);            // Callback to confirm program exit
  fullscreen();                       // Fill screen, no window manager border
  Fl::visual(FL_DOUBLE | FL_INDEX);   // X double buffer extension library

//  set_flag(FORCE_POSITION
//         | OVERRIDE
//         | VISIBLE_FOCUS
//         | MODAL
//         | FULLSCREEN
//    );

  // Do not use:
  //set_override();   // Activates the flags NOBORDER|OVERRIDE

  // Do not appear to help:
//  activate();
//  set_modal();
//  set_visible_focus();
//  take_focus();
//  wait_for_expose();
//  activate();
//  visible_focus(1);
//  set_visible_focus();
//  set_active();
//  std::cout << "take_focus(): " << take_focus() << '\n';

//  Fl_Text_Buffer *buff = new Fl_Text_Buffer();
//  Fl_Text_Display *disp = new Fl_Text_Display((scr.w_px / 2) - 100,
//                                              (scr.h_px / 2) - 100,
//                                              200,
//                                              200, "Eyelib");
//     disp->buffer(buff);
//     buff->text("press Enter to start\n"
//                "press Esc to cancel\n"
//                "right click for options\n"
//                "Alt + F4 to exit");

  fl_cursor(FL_CURSOR_CROSS);         // Mouse cursor
}

Window::Impl::~Impl()
{
  window_lock lock();     // Acquire scoped lock

  // Unregister handlers call handler methods
  //tracker_.register_handler([this](Gaze        const& g){ /* do-nothing */ });
  //tracker_.register_handler([this](Calibration const& c){ /* do-nothing */ });

  // Restore previously registered handlers
  tracker_.register_handler(calib_callback_);
  tracker_.register_handler(gaze_callback_);

  state_ = Window::State::close;
  state_callback_(state_);
}

//---------------------------------------------------------------------------

int
Window::Impl::run()
{
  // Enable multi-thread support by locking from the main
  // thread.  Fl::wait() and Fl::run() call Fl::unlock() and
  // Fl::lock() as needed to release control to the child threads
  // when it is safe to do so...
  if (Fl::lock())
  {
    eye::debug::error(__FILE__, __LINE__, "multi-thread not supported");
  }
  // All callbacks and derived functions like
  // handle() and draw() will now be properly locked.

  //make_current();
  show();   // Can only be called by the main thread

  handle(FL_ENTER);
  handle(FL_PUSH);
  handle(FL_RELEASE);

//  activate();

//  Fl::awake();

  // Save currently registered handlers
  calib_callback_ = tracker_.get_calib_handler();
  gaze_callback_  = tracker_.get_gaze_handler();

  // Lambdas to call handler methods
  tracker_.register_handler([this](Gaze        const& g){ handle(g); });
  tracker_.register_handler([this](Calibration const& c){ handle(c); });

  redraw();                       // Mark window as needing draw() called
  state_ = Window::State::ready;  // Update state
  state_callback_(state_);        // Invoke callback
  tracker_.start();               // Start tracker if it's not already started
  //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return Fl::run();               // Run until window is closed
}

// draw() is called by FLTK (main thread).
// Note: Only need to Fl::lock() and Fl::unlock() for FLTK
// calls made from within a thread that is not the main thread.
void
Window::Impl::draw()  // override
{
  Fl_Double_Window::draw();   // Draw base window
  window::draw(targets_);     // Draw target(s)
  calib_.draw();          // Draw overall calibration or point results
  text_.draw();           // Draw text      ... to hide:  text_.show = false
  gaze_.draw();           // Draw gaze point(s)
}

//---------------------------------------------------------------------------

// handle() is called by FLTK (main thread).
// Note: Only need to Fl::lock() and Fl::unlock() for FLTK
// calls made from within a thread that is not the main thread.
int
Window::Impl::handle(int event_code)  // override
{
  // 2016-09-25 Lucas:  Notwithstanding comment above, added
  //  scoped lock to address crashes during use of the popup menu.
  window_lock lock();   // Acquire scoped lock

  eye::Window::Event event(tracker_.gaze_time_ms(), event_code);  // Event data
  bool handled = false;                   // true if event is handled
  event_callback_(event);                 // Event notification callback

  if (event.is_focus())
  {
    handled = true;
  }
  else if (event.mouse.is_release() && event.mouse.is_button_right())
  {
    handle_right_click();
    handled = true;
  }
  else if (event.key.is_press())
  {
    handled = handle_key_press(event);    // Handle key press
  }
  else if (event.key.is_release())
  {
    handled = true;         // Consume all key release events
  }

  if (handled)
  {
    redraw();     // Mark window as needing draw() called
  }
  // Tell FLTK if event was handled; otherwise pass event to window
  return (handled ? Event::HANDLED : Fl_Double_Window::handle(event_code));
}

bool
Window::Impl::handle_key_press(Event const& event)
{
  using State   = eye::Window::State;
  using Special = eye::Window::Event::Key::Special;

  // Handle key press for any window state
  if (event.key.to_string() == "1")
  {
    toggle_raw_gaze(0, (void*)this);
    //gaze_.show_raw = !gaze_.show_raw;   // Toggle raw gaze widget
    return true;
  }
  if (event.key.to_string() == "2")
  {
    toggle_avg_gaze(0, (void*)this);
    //gaze_.show_avg = !gaze_.show_avg;   // Toggle smoothed gaze widget
    return true;
  }
  if ((event.key.to_string() == "c") ||
      (event.key.to_string() == "C"))
  {
    // Change which calibration results to show
    //  average --> all points --> none
    using S = eye::window::CalibWidget::Show;
    switch (calib_.show())
    {
      case S::average:  calib_.show(S::points);   break;
      case S::points:   calib_.show(S::none);     break;
      case S::none:     calib_.show(S::average);  break;
      default:  break;  // Invalid
    }
    return true;
  }
  // Alt+F4 or Ctrl+F4 to close window
  if ((Event::State::Key::alt() || Event::State::Key::ctrl()) &&
      (event.key.function() == 4))
  {
    exit_callback(0, 0);
    return true;    // Consume event
  }

  // Handle key press based on window state
  //    init --> ready --> active --> close
  switch (state_)
  {
    case State::ready:
      if (event.key.special() == Special::escape)   // Esc key
      {
        //return false;   // Pass to window
        return true;   // Consume
      }
      else if (event.key.is_enter())   // Enter key
      {
        text_.show = false;
        state_ = State::active;
        state_callback_(state_);      // Invoke callback
        // Hide calibration results
        calib_.show(eye::window::CalibWidget::Show::none);
      }
      break;
    case State::active:
      if (event.key.special() == Special::escape)   // Esc key
      {
        text_.show = true;
        state_ = State::ready;
        state_callback_(state_);      // Invoke callback
      }
    case State::init:   // Window not running yet
    case State::close:  // Window is closing
    default:  break;    // Do nothing
  }
  return true;    // Consume event
}

void
Window::Impl::handle_right_click()
{
  int raw_gaze_flags = (FL_MENU_TOGGLE | (gaze_.show_raw ? FL_MENU_VALUE : 0));
  int avg_gaze_flags = (FL_MENU_TOGGLE | (gaze_.show_avg ? FL_MENU_VALUE : 0));

  using S = eye::window::CalibWidget::Show;
  auto s = calib_.show();
  int calib_avg_flags = (FL_MENU_RADIO | (s==S::average ? FL_MENU_VALUE : 0));
  int calib_pts_flags = (FL_MENU_RADIO | (s==S::points  ? FL_MENU_VALUE : 0));
  int calib_non_flags = (FL_MENU_RADIO | (s==S::none    ? FL_MENU_VALUE : 0));

  //, FL_NORMAL_LABEL, FL_HELVETICA

  //---------------------------------------------------------------
  //  struct Fl_Menu_Item
  //  {
  //    const char*   text; // label()
  //    ulong         shortcut;
  //    Fl Callback*  callback;
  //    void*         user_data;
  //    int           flags;
  //    uchar         labeltype;
  //    uchar         labelfont;
  //    uchar         labelsize;
  //    uchar         labelcolor;
  //  };
  //---------------------------------------------------------------
  Fl_Menu_Item menu[] = {
    { "Calibration:", 'c', 0, 0, FL_MENU_INACTIVE },
    { "&Average",     0, show_calib_average, (void*)this, calib_avg_flags },
    { "&Points",      0, show_calib_points,  (void*)this, calib_pts_flags },
    { "&None",        0, show_calib_none,    (void*)this, calib_non_flags |
                                                            FL_MENU_DIVIDER },
    { "Gaze:",        0, 0, 0, FL_MENU_INACTIVE },
    { "&Raw",         '1',  toggle_raw_gaze, (void*)this, raw_gaze_flags },
    { "&Smoothed",    '2',  toggle_avg_gaze, (void*)this, avg_gaze_flags |
                                                            FL_MENU_DIVIDER },
    { "E&xit",        FL_ALT + FL_F + 4, exit_callback },
    { 0 }
  };

  Fl_Menu_Item const* m = menu->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);

  if (m)
  {
    m->do_callback(0, m->user_data());
  }
}

//---------------------------------------------------------------------------

void
Window::Impl::handle(Calibration const& c)
{
  window_lock lock();   // Acquire scoped lock
  calib_.set(c);        // Set calibration results
  calib_callback_(c);   // Invoke saved callback

  // Show results when drawing
  calib_.show(eye::window::CalibWidget::Show::average);

  text_.show  = true;
  state_      = State::ready;
  state_callback_(state_);      // Invoke callback
  redraw();       // Mark window as needing draw() called
  Fl::awake();    // Tell main thread to redraw
}

void
Window::Impl::handle(eye::Gaze const& g)
{
  //-------------------------------------------------------
  // Fl::flush() causes all windows that need it to be
  // redrawn and graphics forced out through the pipes.
  // This is what wait() does before looking for events.
  // Note:  In multi-threaded applications you should only
  //   call Fl::flush() from the main thread.  If a child
  //   thread needs to trigger a redraw event, it should
  //   instead call Fl::awake() to get the main thread to
  //   process the event queue.
  //-------------------------------------------------------
  window_lock lock();   // Acquire scoped lock
  gaze_.set(g);         // Set gaze point coordinates and fixation flag
 #if 1
  gaze_callback_(g);    // Invoke saved callback
 #endif
  redraw();             // Mark window as needing draw() called
  Fl::awake();          // Tell main thread to redraw
}

//---------------------------------------------------------------------------

/*static*/ void
Window::Impl::toggle_raw_gaze(Fl_Widget*, void *userdata)
{
  Window::Impl *w = (Window::Impl*)userdata;
  w->gaze_.show_raw = !w->gaze_.show_raw;   // Toggle raw gaze widget
}

/*static*/ void
Window::Impl::toggle_avg_gaze(Fl_Widget*, void *userdata)
{
  Window::Impl *w = (Window::Impl*)userdata;
  w->gaze_.show_avg = !w->gaze_.show_avg;   // Toggle smoothed gaze widget
}

/*static*/ void
Window::Impl::show_calib_average(Fl_Widget*, void *userdata)
{
  Window::Impl *w = (Window::Impl*)userdata;
  w->calib_.show(eye::window::CalibWidget::Show::average);
}

/*static*/ void
Window::Impl::show_calib_points(Fl_Widget*, void *userdata)
{
  Window::Impl *w = (Window::Impl*)userdata;
  w->calib_.show(eye::window::CalibWidget::Show::points);
}

/*static*/ void
Window::Impl::show_calib_none(Fl_Widget*, void *userdata)
{
  Window::Impl *w = (Window::Impl*)userdata;
  w->calib_.show(eye::window::CalibWidget::Show::none);
}


/////////////////////////////////////////////////////////////////////////////
//  Window Class
/////////////////////////////////////////////////////////////////////////////

Window::Window(Tracker& tracker, Screen const& scr,
               std::string const& title, ColorRGB const& bg)
: pimpl(utl::make_unique<Impl>(tracker, scr, title, bg))
{}

Window::~Window()
{}

int
Window::run()
{
  std::cout << "time_ms,event,x,y,state\n";  // Output header
  return pimpl->run();    // Run until window is closed
}

//-------------------------------------------------------------
// Set Properties
//-------------------------------------------------------------

void
Window::clear_targets()
{
  window_lock lock();       // Acquire scoped lock
  std::cout << (std::to_string(pimpl->tracker_.gaze_time_ms()) +
                ",clear_targets\n");
  pimpl->targets_.clear();  // Clear content of container
  pimpl->redraw();          // Mark window as needing draw() called
  Fl::awake();              // Tell main thread to redraw
}

void
Window::set(Target const& t)
{
  window_lock lock();       // Acquire scoped lock
  std::cout << pimpl->tracker_.gaze_time_ms() << ",target," << t << '\n';
  pimpl->targets_ = {t};    // Single target in container
  pimpl->redraw();          // Mark window as needing draw() called
  Fl::awake();              // Tell main thread to redraw
}

void
Window::set(Targets const& ts)
{
  if (ts.empty())
  {
    clear_targets();
    return;
  }
  window_lock lock();   // Acquire scoped lock

  // Range construct from content of t
  pimpl->targets_ = window::TargetWidgets(ts.cbegin(), ts.cend());

  pimpl->redraw();      // Mark window as needing draw() called
  Fl::awake();          // Tell main thread to redraw
}

//void
//Window::show_calib(bool val)
//{
//  window_lock lock();   // Acquire scoped lock
//  pimpl->show_calib_ = val;
//  pimpl->redraw();      // Mark window as needing draw() called
//  Fl::awake();          // Tell main thread to redraw
//}

void
Window::show_avg_gaze(bool val)
{
  window_lock lock();   // Acquire scoped lock
  pimpl->gaze_.show_avg = val;
  pimpl->redraw();      // Mark window as needing draw() called
  Fl::awake();          // Tell main thread to redraw
}

void
Window::show_raw_gaze(bool val)
{
  window_lock lock();   // Acquire scoped lock
  pimpl->gaze_.show_raw = val;
  pimpl->redraw();      // Mark window as needing draw() called
  Fl::awake();          // Tell main thread to redraw
}

//-------------------------------------------------------------
// Operations
//-------------------------------------------------------------

void
Window::redraw() const
{
  window_lock lock();   // Acquire scoped lock
  pimpl->redraw();      // Mark window as needing draw() called
  Fl::awake();          // Tell main thread to redraw
}

void
Window::register_handler(event_handler callback)
{
  window_lock lock();   // Acquire scoped lock
  if (callback)
  {
    pimpl->event_callback_ = callback;
  }
}

void
Window::register_handler(state_handler callback)
{
  window_lock lock();   // Acquire scoped lock
  if (callback)
  {
    pimpl->state_callback_ = callback;
  }
}


/////////////////////////////////////////////////////////////////////////////

} // eye
//===========================================================================//
