//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/menu.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  enum class WindowMode {
    Windowed,
    Borderless
  };

  enum class WindowState {
    Normal,
    Minimized,
    Maximized
  };

  struct WindowSettings {
    String title = "Hyperion";
    String icon = "icon/icon.ico";

    uint32 width = 1280;
    uint32 height = 720;

    uint32 min_width = 960;
    uint32 min_height = 540;

    WindowMode window_mode = WindowMode::Windowed;
    WindowState window_state = WindowState::Normal;

    Menu menu;
    bool8 hide_menu_in_borderless_mode = true;
  };

}
