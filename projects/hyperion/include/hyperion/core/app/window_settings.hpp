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

    enum class VSyncMode {
        DontSync,
        EveryVBlank,
        EverySecondVBlank
    };

    struct WindowSettings {
        String title = "Hyperion";
        String icon = "icon/icon.ico";

        u32 width = 1280;
        u32 height = 720;

        WindowMode window_mode = WindowMode::Windowed;
        VSyncMode vsync_mode = VSyncMode::EveryVBlank;
    };

}