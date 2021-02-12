//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/rect.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Rect::Rect() { }

    //--------------------------------------------------------------
    Rect::Rect(Vec2 position, Vec2 size)
        : position(position), size(size) { }

    //--------------------------------------------------------------
    Rect::Rect(float32 x, float32 y, float32 width, float32 height)
        : x(x), y(y), width(width), height(height) { }

    //--------------------------------------------------------------
    Vec2 Rect::GetMin() const {
        return position - size / 2.0f;
    }

    //--------------------------------------------------------------
    Vec2 Rect::GetMax() const {
        return position + size / 2.0f;
    }

    //--------------------------------------------------------------
    bool Rect::Intersects(Rect rect) const {
        return (size > rect.position && position < rect.size) || (position > rect.size && size < rect.position);;
    }

    //--------------------------------------------------------------
    bool Rect::Contains(Vec2 point) const {
        return point > GetMin() && point < GetMax();
    }

    //--------------------------------------------------------------
    String Rect::ToString() const {
        return StringUtils::Format("(Position: {}, Size: {})", position.ToString(), size.ToString());
    }

    //--------------------------------------------------------------
    bool Rect::operator==(const Rect &other) const {
        return position == other.position && size == other.size;
    }

    //--------------------------------------------------------------
    bool Rect::operator!=(const Rect &other) const {
        return !(*this == other);
    }

    //--------------------------------------------------------------
    RectInt::RectInt() { }

    //--------------------------------------------------------------
    RectInt::RectInt(Vec2Int position, Vec2Int size)
        : position(position), size(size) { }

    //--------------------------------------------------------------
    RectInt::RectInt(int32 x, int32 y, int32 width, int32 height)
        : x(x), y(y), width(width), height(height) {
    }

    //--------------------------------------------------------------
    Vec2Int RectInt::GetMin() const {
        return position - size / 2;
    }

    //--------------------------------------------------------------
    Vec2Int RectInt::GetMax() const {
        return position + size / 2;
    }

    //--------------------------------------------------------------
    bool RectInt::Intersects(RectInt rect) const {
        return (size > rect.position && position < rect.size) || (position > rect.size && size < rect.position);;
    }

    //--------------------------------------------------------------
    bool RectInt::Contains(Vec2Int point) const {
        return point > GetMin() && point < GetMax();
    }

    //--------------------------------------------------------------
    String RectInt::ToString() const {
        return StringUtils::Format("(Position: {}, Size: {})", position.ToString(), size.ToString());
    }

    //--------------------------------------------------------------
    bool RectInt::operator==(const RectInt &other) const {
        return position == other.position && size == other.size;
    }

    //--------------------------------------------------------------
    bool RectInt::operator!=(const RectInt &other) const {
        return !(*this == other);
    }

}