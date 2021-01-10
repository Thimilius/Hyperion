#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {
    class Engine;
}

namespace Hyperion {

    class Time final {
    public:
        inline static f32 GetMaxDeltaTime() { return s_max_delta_time; }
        inline static f32 GetFixedDeltaTime() { return s_fixed_delta_time; }

        inline static f32 GetDeltaTime() { return s_delta_time; }
        inline static f64 GetTime() { return s_time; }

        inline static u32 GetFPS() { return s_fps; }
        inline static f64 GetFrameTime() { return s_frame_time; }
    private:
        Time() = delete;
        ~Time() = delete;
    private:
        inline static f32 s_max_delta_time = 0;
        inline static f32 s_fixed_delta_time = 0;

        inline static f32 s_delta_time = 0;
        inline static f64 s_time = 0;

        inline static u32 s_fps = 0;
        inline static f64 s_frame_time = 0;
    private:
        friend class Hyperion::Engine;
    };

}