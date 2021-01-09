#pragma once

#include "hyperion/common.hpp"

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

        u32 width = 1280;
        u32 height = 720;

        u32 min_width = 640;
        u32 min_height = 360;

        WindowMode window_mode = WindowMode::Windowed;
    };

}