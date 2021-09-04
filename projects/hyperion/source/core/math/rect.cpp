//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/rect.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Rect::Rect() { }

    //--------------------------------------------------------------
    Rect::Rect(Vector2 position, Vector2 size)
        : position(position), size(size) { }

    //--------------------------------------------------------------
    Rect::Rect(float32 x, float32 y, float32 width, float32 height)
        : x(x), y(y), width(width), height(height) { }

    //--------------------------------------------------------------
    Vector2 Rect::GetMin() const {
        return Vector2(x, y);
    }

    //--------------------------------------------------------------
    Vector2 Rect::GetMax() const {
        return Vector2(x + width, y + height);
    }

    //--------------------------------------------------------------
    Vector2 Rect::GetCenter() const {
        return Vector2(x + width / 2.0f, y + height / 2.0f);
    }

    //--------------------------------------------------------------
    bool8 Rect::Intersects(const Rect &other) const {
        Vector2 this_max = GetMax();
        Vector2 other_max = other.GetMax();
        return other_max.x > x && other.x < this_max.x && other_max.y > y && other.y < this_max.y;
    }

    //--------------------------------------------------------------
    bool8 Rect::Contains(Vector2 point) const {
        Vector2 max = GetMax();
        return point.x >= x && point.x < max.x && point.y >= y && point.y < max.y;
    }

    //--------------------------------------------------------------
    String Rect::ToString() const {
        return StringUtils::Format("(Position: {}, Size: {})", position.ToString(), size.ToString());
    }

    //--------------------------------------------------------------
    bool8 Rect::operator==(const Rect &other) const {
        return position == other.position && size == other.size;
    }

    //--------------------------------------------------------------
    bool8 Rect::operator!=(const Rect &other) const {
        return !(*this == other);
    }

    //--------------------------------------------------------------
    RectInt::RectInt() { }

    //--------------------------------------------------------------
    RectInt::RectInt(Vector2Int position, Vector2Int size)
        : position(position), size(size) { }

    //--------------------------------------------------------------
    RectInt::RectInt(int32 x, int32 y, int32 width, int32 height)
        : x(x), y(y), width(width), height(height) { }

    //--------------------------------------------------------------
    Vector2Int RectInt::GetMin() const {
        return Vector2Int(x, y);
    }

    //--------------------------------------------------------------
    Vector2Int RectInt::GetMax() const {
        return Vector2Int(x + width, y + height);
    }

    //--------------------------------------------------------------
    Vector2Int RectInt::GetCenter() const {
        return Vector2Int(x + width / 2, y + height / 2);
    }

    //--------------------------------------------------------------
    bool8 RectInt::Intersects(const RectInt &other) const {
        Vector2Int this_max = GetMax();
        Vector2Int other_max = other.GetMax();
        return other_max.x > x && other.x < this_max.x &&other_max.y > y && other.y < this_max.y;
    }

    //--------------------------------------------------------------
    bool8 RectInt::Contains(Vector2Int point) const {
        Vector2Int max = GetMax();
        return point.x >= x && point.x < max.x &&point.y >= y && point.y < max.y;
    }

    //--------------------------------------------------------------
    String RectInt::ToString() const {
        return StringUtils::Format("(Position: {}, Size: {})", position.ToString(), size.ToString());
    }

    //--------------------------------------------------------------
    bool8 RectInt::operator==(const RectInt &other) const {
        return position == other.position && size == other.size;
    }

    //--------------------------------------------------------------
    bool8 RectInt::operator!=(const RectInt &other) const {
        return !(*this == other);
    }

}