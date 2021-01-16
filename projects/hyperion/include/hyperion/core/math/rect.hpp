#pragma once

#include "hyperion/core/math/vec2.hpp"

namespace Hyperion {

    struct Rect {
        union {
            Vec2 position;
            struct {
                float32 x;
                float32 y;
            };
        };

        union {
            Vec2 size;
            struct {
                float32 width;
                float32 height;
            };
        };

        Rect();
        Rect(Vec2 position, Vec2 size);
        Rect(float32 x, float32 y, float32 width, float32 height);

        Vec2 GetMin() const;
        Vec2 GetMax() const;

        bool Intersects(Rect bounds) const;
        bool Contains(Vec2 point) const;

        String ToString() const;

        bool operator==(const Rect &other) const;
        bool operator!=(const Rect &other) const;
    };

}