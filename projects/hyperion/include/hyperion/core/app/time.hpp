#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/math/math.hpp"

namespace Hyperion {
    class Engine;
}

namespace Hyperion {

    class Time final {
    public:
        inline static f32 GetMaxDeltaTime() { return s_max_delta_time; }
        inline static f32 GetFixedDeltaTime() { return s_fixed_delta_time; }
        inline static f32 GetDeltaTime() { return s_delta_time; }
        inline static f32 GetTime() { return s_time; }
        inline static f32 GetFrameTime() { return s_frame_time; }
        inline static u32 GetFPS() { return s_fps; }

        inline static bool OnInterval(f32 interval, f32 offset = 0.0f) {
            return Math::Floor((s_time - offset - s_delta_time) / interval) < Math::Floor((s_time - offset) / interval);
        }

        inline static bool OnTime(f32 timestamp) {
            return s_time >= timestamp && s_time - s_delta_time < timestamp;
        }

        inline static bool BetweenInterval(f32 interval, f32 offset = 0.0f) {
            return Math::ModF(s_time - offset, interval * 2) >= interval;
        }
    private:
        Time() = delete;
        ~Time() = delete;
    private:
        inline static f32 s_max_delta_time = 0.0f;
        inline static f32 s_fixed_delta_time = 0.0f;
        inline static f32 s_delta_time = 0.0f;
        inline static f32 s_time = 0.0f;
        inline static f32 s_frame_time = 0.0f;
        inline static u32 s_fps = 0;
    private:
        friend class Hyperion::Engine;
    };

}