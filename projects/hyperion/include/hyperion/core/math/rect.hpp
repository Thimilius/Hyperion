//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vector2.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct Rect {
        union {
            Vector2 position;
            struct {
                float32 x;
                float32 y;
            };
        };

        union {
            Vector2 size;
            struct {
                float32 width;
                float32 height;
            };
        };

        Rect();
        Rect(Vector2 position, Vector2 size);
        Rect(float32 x, float32 y, float32 width, float32 height);

        Vector2 GetMin() const;
        Vector2 GetMax() const;
        Vector2 GetCenter() const;

        bool8 Intersects(const Rect &other) const;
        bool8 Contains(Vector2 point) const;

        String ToString() const;

        bool8 operator==(const Rect &other) const;
        bool8 operator!=(const Rect &other) const;
    };

    struct RectInt {
        union {
            Vector2Int position;
            struct {
                int32 x;
                int32 y;
            };
        };

        union {
            Vector2Int size;
            struct {
                int32 width;
                int32 height;
            };
        };

        RectInt();
        RectInt(Vector2Int position, Vector2Int size);
        RectInt(int32 x, int32 y, int32 width, int32 height);

        Vector2Int GetMin() const;
        Vector2Int GetMax() const;
        Vector2Int GetCenter() const;

        bool8 Intersects(const RectInt &other) const;
        bool8 Contains(Vector2Int point) const;

        String ToString() const;

        bool8 operator==(const RectInt &other) const;
        bool8 operator!=(const RectInt &other) const;
    };

}