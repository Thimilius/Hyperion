#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class Time {
    private:
        inline static f32 s_max_delta_time = 0;

        inline static f32 s_delta_time = 0;
        inline static f64 s_time = 0;
        
        inline static u32 s_fps = 0;
        inline static f64 s_frame_time = 0;
    public:
        inline static f32 GetMaxDeltaTime() { return s_max_delta_time; }

        inline static f32 GetDeltaTime() { return s_delta_time; }
        inline static f64 GetTime() { return s_time; }

        inline static u32 GetFPS() { return s_fps; }
        inline static f64 GetFrameTime() { return s_frame_time; }
    private:
        Time() = delete;
        ~Time() = delete;

        friend class Application;
    };

}