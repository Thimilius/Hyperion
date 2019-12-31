#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    struct Vec4;

    struct Color {
        f32 r;
        f32 g;
        f32 b;
        f32 a;

        Color();
        Color(f32 r, f32 g, f32 b, f32 a);

        String ToString() const;

        bool operator==(const Color &other) const;
        bool operator!=(const Color &other) const;

        Color &operator+=(const f32 &value);
        Color &operator-=(const f32 &value);
        Color &operator*=(const f32 &value);
        Color &operator/=(const f32 &value);

        operator Vec4();

        inline static Color Black() { return Color(0, 0, 0, 1); }
        inline static Color Red() { return Color(1, 0, 0, 1); }
        inline static Color Green() { return Color(0, 1, 0, 1); }
        inline static Color Blue() { return Color(0, 0, 1, 1); }
        inline static Color Yellow() { return Color(1, 1, 0, 1); }
        inline static Color Magenta() { return Color(1, 0, 1, 1); }
        inline static Color Cyan() { return Color(0, 1, 1, 1); }
        inline static Color Grey() { return Color(0.5f, 0.5f, 0.5f, 1); }
        inline static Color White() { return Color(1, 1, 1, 1); }
        inline static Color Transparent() { return Color(0, 0, 0, 0); }

        friend Color operator+(Color left, f32 right);
        friend Color operator-(Color left, f32 right);
        friend Color operator*(Color left, f32 right);
        friend Color operator/(Color left, f32 right);

        friend Color operator+(f32 left, Color right);
        friend Color operator-(f32 left, Color right);
        friend Color operator*(f32 left, Color right);
        friend Color operator/(f32 left, Color right);
    };

}