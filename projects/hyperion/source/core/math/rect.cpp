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
        return Vec2(x, y);
    }

    //--------------------------------------------------------------
    Vec2 Rect::GetMax() const {
        return Vec2(x + width, y + height);
    }

    //--------------------------------------------------------------
    Vec2 Rect::GetCenter() const {
        return Vec2(x + width / 2.0f, y + height / 2.0f);
    }

    //--------------------------------------------------------------
    bool Rect::Intersects(const Rect &other) const {
        Vec2 this_max = GetMax();
        Vec2 other_max = other.GetMax();
        return other_max.x > x && other.x < this_max.x && other_max.y > y && other.y < this_max.y;
    }

    //--------------------------------------------------------------
    bool Rect::Contains(Vec2 point) const {
        Vec2 max = GetMax();
        return point.x >= x && point.x < max.x && point.y >= y && point.y < max.y;
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
        : x(x), y(y), width(width), height(height) { }

    //--------------------------------------------------------------
    Vec2Int RectInt::GetMin() const {
        return Vec2Int(x, y);
    }

    //--------------------------------------------------------------
    Vec2Int RectInt::GetMax() const {
        return Vec2Int(x + width, y + height);
    }

    //--------------------------------------------------------------
    Vec2Int RectInt::GetCenter() const {
        return Vec2Int(x + width / 2, y + height / 2);
    }

    //--------------------------------------------------------------
    bool RectInt::Intersects(const RectInt &other) const {
        Vec2Int this_max = GetMax();
        Vec2Int other_max = other.GetMax();
        return other_max.x > x && other.x < this_max.x &&other_max.y > y && other.y < this_max.y;
    }

    //--------------------------------------------------------------
    bool RectInt::Contains(Vec2Int point) const {
        Vec2Int max = GetMax();
        return point.x >= x && point.x < max.x &&point.y >= y && point.y < max.y;
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