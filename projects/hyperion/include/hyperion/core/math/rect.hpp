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

    struct RectInt {
        union {
            Vec2Int position;
            struct {
                int32 x;
                int32 y;
            };
        };

        union {
            Vec2Int size;
            struct {
                int32 width;
                int32 height;
            };
        };

        RectInt();
        RectInt(Vec2Int position, Vec2Int size);
        RectInt(int32 x, int32 y, int32 width, int32 height);

        Vec2Int GetMin() const;
        Vec2Int GetMax() const;

        bool Intersects(RectInt bounds) const;
        bool Contains(Vec2Int point) const;

        String ToString() const;

        bool operator==(const RectInt &other) const;
        bool operator!=(const RectInt &other) const;
    };

}