//===========================================================================//
//  Nathan Lucas
//  2015-2016
//===========================================================================//

#include <eyelib/screen.hpp>

#include <FL/Fl.H>          // FLTK GUI libraries

#include <algorithm>  // std::min
#include <tuple>      // std::tie
#include <sstream>    // std::ostringstream
#include <iostream>   // std::cout

namespace {   //-------------------------------------------------------------

inline int
double_to_int(double d)   // round double to int
{
  return static_cast<int>(d + 0.5);
}

} // anonymous --------------------------------------------------------------

namespace eye {
//---------------------------------------------------------------------------

Screen::Screen(unsigned index, int x_px, int y_px,
               unsigned w_px, unsigned h_px, float w_m, float h_m)
: index(index), x_px(x_px), y_px(y_px)
, w_px(w_px), h_px(h_px), w_m(w_m), h_m(h_m)
{}

//---------------------------------------------------------------------------

Screen
screen(unsigned index)
{
  // Validate screen index
  int n = Fl::screen_count();
  if ((n < 1) || (index >= static_cast<unsigned>(n)))
  {
    return Screen{};
  }

  // Get the bounding box of the work area of the given screen
  // Note: No window position and size offsets due to fullscreen mode
  int x=0, y=0, w=0, h=0;
  Fl::screen_work_area(x, y, w, h, index);

  // Validate screen width and height
  if ((w < 0) || (h < 0))
  {
    std::cout << "ERROR: invalid screen size: " << w << " x " << h << '\n';
    return Screen{};
  }

  // Get screen resolution in dots-per-inch
  float screen_dpi_h = 0.0;
  float screen_dpi_v = 0.0;
  Fl::screen_dpi(screen_dpi_h, screen_dpi_v, index);

  // Compute physical screen size in meters
  float wm = (0.0254 * (w / screen_dpi_h));
  float hm = (0.0254 * (h / screen_dpi_v));

  return Screen(index, x, y, w, h, wm, hm);
}

std::string
screen_list(unsigned index)
{
  std::ostringstream oss(std::ostringstream::ate);

  // Validate screen index
  int screen_count = Fl::screen_count();
  if (screen_count < 0)
  {
    oss << "ERROR: invalid screen count: " << screen_count << '\n';
    return oss.str();
  }

  // Accumulate parameters for each screen
  for (unsigned i = 0; i != static_cast<unsigned>(screen_count); ++i)
  {
    int x=0, y=0, w=0, h=0;
    Fl::screen_xywh(x, y, w, h, i);
    oss << "[" << ((i == index) ? "x" : " ") << "] screen " << i
        << " : " << w << " x " << h
        << " (" << x << ", " << y << ")" << '\n';
  }
  return oss.str();
}

bool
operator==(Screen const& lhs, Screen const& rhs)
{
  return std::tie(lhs.index, lhs.x_px, lhs.y_px,
                  lhs.w_px, lhs.h_px, lhs.w_m, lhs.h_m)
      == std::tie(rhs.index, rhs.x_px, rhs.y_px,
                  rhs.w_px, rhs.h_px, rhs.w_m, rhs.h_m);
}

bool
operator!=(Screen const& lhs, Screen const& rhs)
{
  return !(lhs == rhs);
}

std::ostream&
operator<<(std::ostream& os, Screen const& s)
{
  return os << "screen index       : "  << s.index
    << '\n' << "position (pixel)   : (" << s.x_px << ", "<< s.y_px <<")"
    << '\n' << "resolution (pixel) : "  << s.w_px <<" x "<< s.h_px
    << '\n' << "dimensions (meter) : "  << s.w_m  <<" x "<< s.h_m;
}

//---------------------------------------------------------------------------

std::ostream&
operator<<(std::ostream& os, Target const& t)
{
  return os << t.x_px << ',' << t.y_px << ','
            << (t.active ? "active" : "inactive");
}

Targets
target_array(Screen const& scr, unsigned x, unsigned y)
{
  // Margin equal to 10% of minimum screen dimension
  auto margin = (0.10 * std::min(scr.w_px, scr.h_px));

  // Horizontal and vertical spacing
  auto dx = static_cast<double>(scr.w_px - 2*margin) / (x-1);
  auto dy = static_cast<double>(scr.h_px - 2*margin) / (y-1);

  Targets targets;

  // Space targets at regular intervals across the screen
  for (unsigned yi = 0; yi != y; ++yi)
  {
    for (unsigned xi = 0; xi != x; ++xi)
    {
      int xx = double_to_int((dx * xi) + margin);
      int yy = double_to_int((dy * yi) + margin);
      targets.push_back({ xx, yy, true });
    }
  }
  return targets;
}

Targets
target_array(Screen const& scr, unsigned x, unsigned y,
             unsigned margin_x, unsigned margin_y)
{
  // Horizontal and vertical spacing
  auto dx = static_cast<double>(scr.w_px - 2*margin_x) / (x-1);
  auto dy = static_cast<double>(scr.h_px - 2*margin_y) / (y-1);

  Targets targets;

  // Space targets at regular intervals across the screen
  for (unsigned yi = 0; yi != y; ++yi)
  {
    for (unsigned xi = 0; xi != x; ++xi)
    {
      int xx = double_to_int((dx * xi) + margin_x);
      int yy = double_to_int((dy * yi) + margin_y);
      targets.push_back({ xx, yy, true });
    }
  }
  return targets;
}

//---------------------------------------------------------------------------
} // eye
//===========================================================================//
