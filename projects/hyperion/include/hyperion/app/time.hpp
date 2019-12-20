#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class Application;

    class Time {
    private:
        inline static f32 s_max_delta_time = 0;

        inline static f32 s_delta_time = 0;
        inline static f64 s_time = 0;
        
        inline static u32 s_fps = 0;
        inline static f64 s_frame_time = 0;
    public:
        static inline f32 GetMaxDeltaTime() { return s_max_delta_time; }

        static inline f32 GetDeltaTime() { return s_delta_time; }
        static inline f64 GetTime() { return s_time; }

        static inline u32 GetFPS() { return s_fps; }
        static inline f64 GetFrameTime() { return s_frame_time; }
    private:
        Time() = delete;
        ~Time() = delete;

        friend class Application;
    };

}