#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class Application;

    class Time {
    private:
        inline static float s_max_delta_time = 0;

        inline static float s_delta_time = 0;
        inline static double s_time = 0;
        
        inline static u32 s_fps = 0;
        inline static double s_frame_time = 0;
    public:
        static inline float GetMaxDeltaTime() { return s_max_delta_time; }

        static inline float GetDeltaTime() { return s_delta_time; }
        static inline double GetTime() { return s_time; }

        static inline u32 GetFPS() { return s_fps; }
        static inline double GetFrameTime() { return s_frame_time; }
    private:
        Time() = delete;
        ~Time() = delete;

        friend class Application;
    };

}