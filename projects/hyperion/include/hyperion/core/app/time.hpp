#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/core/math/math.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Time final {
    public:
        inline static float32 GetMaxDeltaTime() { return s_max_delta_time; }
        inline static float32 GetFixedDeltaTime() { return s_fixed_delta_time; }
        inline static float32 GetDeltaTime() { return s_delta_time; }
        inline static float32 GetTime() { return s_time; }
        inline static float32 GetTimeSinceEngineModeChange() { return s_time_since_engine_mode_change; }
        inline static float32 GetFrameTime() { return s_frame_time; }
        inline static uint32 GetFPS() { return s_fps; }

        inline static bool OnInterval(float32 interval, float32 offset = 0.0f) {
            return Math::Floor((s_time - offset - s_delta_time) / interval) < Math::Floor((s_time - offset) / interval);
        }

        inline static bool OnTime(float32 timestamp) {
            return s_time >= timestamp && s_time - s_delta_time < timestamp;
        }

        inline static bool BetweenInterval(float32 interval, float32 offset = 0.0f) {
            return Math::ModF(s_time - offset, interval * 2) >= interval;
        }
    private:
        Time() = delete;
        ~Time() = delete;
    private:
        inline static float32 s_max_delta_time = 0.0f;
        inline static float32 s_fixed_delta_time = 0.0f;
        inline static float32 s_delta_time = 0.0f;

        inline static float32 s_time = 0.0f;
        inline static float32 s_time_since_engine_mode_change = 0.0f;
        
        inline static uint64 s_frame_counter = 0;
        inline static float32 s_frame_time = 0.0f;
        inline static uint32 s_fps = 0;
        inline static const uint64 MAX_PAST_DELTA_TIMES = 1024;
        inline static float32 s_past_delta_times[MAX_PAST_DELTA_TIMES];
    private:
        friend class Hyperion::Engine;
    };

}