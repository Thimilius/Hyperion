//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

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

        uint32 min_width = 640;
        uint32 min_height = 360;

        WindowMode window_mode = WindowMode::Windowed;
    };

}