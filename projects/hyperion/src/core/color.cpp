#include "hyppch.hpp"

#include "hyperion/core/color.hpp"

#include "hyperion/core/math/vec4.hpp"

namespace Hyperion {

    Color::Color() { }

    Color::Color(f32 r, f32 g, f32 b, f32 a) 
        : r(r), g(g), b(b), a(a) { }

    String Color::ToString() const {
        return StringUtils::Format("({}, {}, {}, {})", r, g, b, a);
    }

    bool Color::operator==(const Color &other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    bool Color::operator!=(const Color &other) const {
        return !(*this == other);
    }

    Color &Color::operator+=(const f32 &value) {
        r += value;
        g += value;
        b += value;
        a += value;

        return *this;
    }

    Color &Color::operator-=(const f32 &value) {
        r -= value;
        g -= value;
        b -= value;
        a -= value;

        return *this;
    }

    Color &Color::operator*=(const f32 &value) {
        r *= value;
        g *= value;
        b *= value;
        a *= value;

        return *this;
    }

    Color &Color::operator/=(const f32 &value) {
        r /= value;
        g /= value;
        b /= value;
        a /= value;

        return *this;
    }

    Color::operator Vec4() {
        return Vec4(r, g, b, a);
    }

    Color operator+(Color left, f32 right) {
        return left += right;
    }

    Color operator-(Color left, f32 right) {
        return left -= right;
    }

    Color operator*(Color left, f32 right) {
        return left *= right;
    }

    Color operator/(Color left, f32 right) {
        return left /= right;
    }

    Color operator+(f32 left, Color right) {
        return right += left;
    }

    Color operator-(f32 left, Color right) {
        return right -= left;
    }

    Color operator*(f32 left, Color right) {
        return right *= left;
    }

    Color operator/(f32 left, Color right) {
        return right /= left;
    }

}