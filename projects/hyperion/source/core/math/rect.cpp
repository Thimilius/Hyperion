#include "hyppch.hpp"

#include "hyperion/core/math/rect.hpp"

namespace Hyperion {

    Rect::Rect() { }

    Rect::Rect(Vec2 position, Vec2 size)
        : position(position), size(size) { }

    Rect::Rect(float32 x, float32 y, float32 width, float32 height) 
        : position(Vec2(x, y)), size(Vec2(width, height)) { }

    Vec2 Rect::GetMin() const {
        return position - size / 2.0f;
    }

    Vec2 Rect::GetMax() const {
        return position + size / 2.0f;
    }

    bool Rect::Intersects(Rect rect) const {
        return (size > rect.position && position < rect.size) || (position > rect.size && size < rect.position);;
    }

    bool Rect::Contains(Vec2 point) const {
        return point > GetMin() && point < GetMax();
    }

    String Rect::ToString() const {
        return StringUtils::Format("(Position: {}, Size: {})", position.ToString(), size.ToString());
    }

    bool Rect::operator==(const Rect &other) const {
        return position == other.position && size == other.size;
    }

    bool Rect::operator!=(const Rect &other) const {
        return !(*this == other);
    }

}