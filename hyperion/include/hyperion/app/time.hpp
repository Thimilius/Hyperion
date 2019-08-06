#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class CApplication;

    class CTime {
    private:
        static float s_delta;
        static double s_time;
        
        static u32 s_fps;
        static double s_frame_time;
    public:
        static inline float GetDelta() { return s_delta; }
        static inline double GetTime() { return s_time; }

        static inline u32 GetFPS() { return s_fps; }
        static inline double GetFrameTime() { return s_frame_time; }
    private:
        CTime() = delete;
        ~CTime() = delete;

        friend class CApplication;
    };

}