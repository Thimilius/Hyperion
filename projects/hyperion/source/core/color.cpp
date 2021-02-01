#include "hyppch.hpp"

#include "hyperion/core/color.hpp"

#include "hyperion/core/math/vec4.hpp"

namespace Hyperion {

    Color::Color() { }

    Color::Color(float32 r, float32 g, float32 b, float32 a) 
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

    Color &Color::operator+=(const float32 &value) {
        r += value;
        g += value;
        b += value;
        a += value;

        return *this;
    }

    Color &Color::operator-=(const float32 &value) {
        r -= value;
        g -= value;
        b -= value;
        a -= value;

        return *this;
    }

    Color &Color::operator*=(const float32 &value) {
        r *= value;
        g *= value;
        b *= value;
        a *= value;

        return *this;
    }

    Color &Color::operator/=(const float32 &value) {
        r /= value;
        g /= value;
        b /= value;
        a /= value;

        return *this;
    }

    Color::operator Vec4() {
        return Vec4(r, g, b, a);
    }

    Color operator+(Color left, float32 right) {
        return left += right;
    }

    Color operator-(Color left, float32 right) {
        return left -= right;
    }

    Color operator*(Color left, float32 right) {
        return left *= right;
    }

    Color operator/(Color left, float32 right) {
        return left /= right;
    }

    Color operator+(float32 left, Color right) {
        return right += left;
    }

    Color operator-(float32 left, Color right) {
        return right -= left;
    }

    Color operator*(float32 left, Color right) {
        return right *= left;
    }

    Color operator/(float32 left, Color right) {
        return right /= left;
    }

}