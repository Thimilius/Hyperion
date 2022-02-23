//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/vector2.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  Vector2::Vector2()
    : x(0), y(0) {
  }

  //--------------------------------------------------------------
  Vector2::Vector2(float32 x, float32 y)
    : x(x), y(y) {
  }

  //--------------------------------------------------------------
  Vector2::Vector2(const Vector3 &Vector3)
    : x(Vector3.x), y(Vector3.y) {
  }

  //--------------------------------------------------------------
  Vector2 &Vector2::Add(const Vector2 &other) {
    x += other.x;
    y += other.y;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2 &Vector2::Subtract(const Vector2 &other) {
    x -= other.x;
    y -= other.y;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2 &Vector2::Multiply(const Vector2 &other) {
    x *= other.x;
    y *= other.y;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2 &Vector2::Divide(const Vector2 &other) {
    x /= other.x;
    y /= other.y;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2 &Vector2::Add(float32 value) {
    x += value;
    y += value;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2 &Vector2::Subtract(float32 value) {
    x -= value;
    y -= value;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2 &Vector2::Multiply(float32 value) {
    x *= value;
    y *= value;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2 &Vector2::Divide(float32 value) {
    x /= value;
    y /= value;

    return *this;
  }

  //--------------------------------------------------------------
  String Vector2::ToString() const {
    return StringUtils::Format("({:.2f}, {:.2f})", x, y);
  }

  //--------------------------------------------------------------
  bool8 Vector2::operator==(const Vector2 &other) const {
    return x == other.x && y == other.y;
  }

  //--------------------------------------------------------------
  bool8 Vector2::operator!=(const Vector2 &other) const {
    return !(*this == other);
  }

  Vector2 Vector2::operator-() {
    return Vector2(-x, -y);
  }

  //--------------------------------------------------------------
  Vector2 &Vector2::operator+=(const Vector2 &other) {
    return Add(other);
  }

  //--------------------------------------------------------------
  Vector2 &Vector2::operator-=(const Vector2 &other) {
    return Subtract(other);
  }

  //--------------------------------------------------------------
  Vector2 &Vector2::operator*=(const Vector2 &other) {
    return Multiply(other);
  }

  //--------------------------------------------------------------
  Vector2 &Vector2::operator/=(const Vector2 &other) {
    return Divide(other);
  }

  //--------------------------------------------------------------
  bool8 Vector2::operator<(const Vector2 &other) const {
    return x < other.x &&y < other.y;
  }

  //--------------------------------------------------------------
  bool8 Vector2::operator<=(const Vector2 &other) const {
    return x <= other.x && y <= other.y;
  }

  //--------------------------------------------------------------
  bool8 Vector2::operator>(const Vector2 &other) const {
    return x > other.x && y > other.y;
  }

  //--------------------------------------------------------------
  bool8 Vector2::operator>=(const Vector2 &other) const {
    return x >= other.x && y >= other.y;
  }

  //--------------------------------------------------------------
  float32 Vector2::Dot(const Vector2 &other) const {
    return x * other.x + y * other.y;
  }

  //--------------------------------------------------------------
  Vector2 Vector2::Normalized() const {
    float32 magnitude = Magnitude();
    if (magnitude == 0) {
      return Vector2();
    } else {
      return Vector2(x / magnitude, y / magnitude);
    }
  }

  //--------------------------------------------------------------
  float32 Vector2::Magnitude() const {
    return Math::Sqrt(x * x + y * y);
  }

  //--------------------------------------------------------------
  float32 Vector2::SqrMagnitude() const {
    return x * x + y * y;
  }

  //--------------------------------------------------------------
  float32 Vector2::Distance(const Vector2 &other) const {
    float32 x = this->x - other.x;
    float32 y = this->y - other.y;

    return Math::Sqrt(x * x + y * y);
  }

  //--------------------------------------------------------------
  Vector2 operator+(Vector2 left, const Vector2 &right) {
    return left.Add(right);
  }

  //--------------------------------------------------------------
  Vector2 operator-(Vector2 left, const Vector2 &right) {
    return left.Subtract(right);
  }

  //--------------------------------------------------------------
  Vector2 operator*(Vector2 left, const Vector2 &right) {
    return left.Multiply(right);
  }

  //--------------------------------------------------------------
  Vector2 operator/(Vector2 left, const Vector2 &right) {
    return left.Divide(right);
  }

  //--------------------------------------------------------------
  Vector2 operator+(Vector2 left, float32 right) {
    return left.Add(right);
  }

  //--------------------------------------------------------------
  Vector2 operator-(Vector2 left, float32 right) {
    return left.Subtract(right);
  }

  //--------------------------------------------------------------
  Vector2 operator*(Vector2 left, float32 right) {
    return left.Multiply(right);
  }

  //--------------------------------------------------------------
  Vector2 operator/(Vector2 left, float32 right) {
    return left.Divide(right);
  }

  //--------------------------------------------------------------
  Vector2 operator+(float32 left, Vector2 right) {
    return right.Add(left);
  }

  //--------------------------------------------------------------
  Vector2 operator-(float32 left, Vector2 right) {
    return right.Subtract(left);
  }

  //--------------------------------------------------------------
  Vector2 operator*(float32 left, Vector2 right) {
    return right.Multiply(left);
  }

  //--------------------------------------------------------------
  Vector2 operator/(float32 left, Vector2 right) {
    return right.Divide(left);
  }

  //--------------------------------------------------------------
  Vector2Int::Vector2Int()
    : x(0), y(0) {
  }

  //--------------------------------------------------------------
  Vector2Int::Vector2Int(int32 x, int32 y)
    : x(x), y(y) {
  }

  //--------------------------------------------------------------
  Vector2Int::Vector2Int(const Vector3Int &Vector3)
    : x(Vector3.x), y(Vector3.y) {
  }

  //--------------------------------------------------------------
  Vector2Int &Vector2Int::Add(const Vector2Int &other) {
    x += other.x;
    y += other.y;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2Int &Vector2Int::Subtract(const Vector2Int &other) {
    x -= other.x;
    y -= other.y;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2Int &Vector2Int::Multiply(const Vector2Int &other) {
    x *= other.x;
    y *= other.y;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2Int &Vector2Int::Divide(const Vector2Int &other) {
    x /= other.x;
    y /= other.y;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2Int &Vector2Int::Add(int32 value) {
    x += value;
    y += value;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2Int &Vector2Int::Subtract(int32 value) {
    x -= value;
    y -= value;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2Int &Vector2Int::Multiply(int32 value) {
    x *= value;
    y *= value;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2Int &Vector2Int::Divide(int32 value) {
    x /= value;
    y /= value;

    return *this;
  }

  //--------------------------------------------------------------
  Vector2 Vector2Int::ToFloat() const {
    return Vector2(static_cast<float32>(x), static_cast<float32>(y));
  }

  //--------------------------------------------------------------
  String Vector2Int::ToString() const {
    return StringUtils::Format("({}, {})", x, y);
  }

  //--------------------------------------------------------------
  bool8 Vector2Int::operator==(const Vector2Int &other) const {
    return x == other.x && y == other.y;
  }

  //--------------------------------------------------------------
  bool8 Vector2Int::operator!=(const Vector2Int &other) const {
    return !(*this == other);
  }

  //--------------------------------------------------------------
  Vector2Int Vector2Int::operator-() {
    return Vector2Int(-x, -y);
  }

  //--------------------------------------------------------------
  Vector2Int &Vector2Int::operator+=(const Vector2Int &other) {
    return Add(other);
  }

  //--------------------------------------------------------------
  Vector2Int &Vector2Int::operator-=(const Vector2Int &other) {
    return Subtract(other);
  }

  //--------------------------------------------------------------
  Vector2Int &Vector2Int::operator*=(const Vector2Int &other) {
    return Multiply(other);
  }

  //--------------------------------------------------------------
  Vector2Int &Vector2Int::operator/=(const Vector2Int &other) {
    return Divide(other);
  }

  //--------------------------------------------------------------
  bool8 Vector2Int::operator<(const Vector2Int &other) const {
    return x < other.x &&y < other.y;
  }

  //--------------------------------------------------------------
  bool8 Vector2Int::operator<=(const Vector2Int &other) const {
    return x <= other.x && y <= other.y;
  }

  //--------------------------------------------------------------
  bool8 Vector2Int::operator>(const Vector2Int &other) const {
    return x > other.x && y > other.y;
  }

  //--------------------------------------------------------------
  bool8 Vector2Int::operator>=(const Vector2Int &other) const {
    return x >= other.x && y >= other.y;
  }

  //--------------------------------------------------------------
  int32 Vector2Int::Dot(const Vector2Int &other) const {
    return x * other.x + y * other.y;
  }

  //--------------------------------------------------------------
  float32 Vector2Int::Magnitude() const {
    float32 x = static_cast<float32>(this->x);
    float32 y = static_cast<float32>(this->y);

    return Math::Sqrt(x * x + y * y);
  }

  //--------------------------------------------------------------
  int32 Vector2Int::SqrMagnitude() const {
    return x * x + y * y;
  }

  //--------------------------------------------------------------
  float32 Vector2Int::Distance(const Vector2Int &other) const {
    float32 x = static_cast<float32>(this->x - other.x);
    float32 y = static_cast<float32>(this->y - other.y);

    return Math::Sqrt(x * x + y * y);
  }

  //--------------------------------------------------------------
  Vector2Int operator+(Vector2Int left, const Vector2Int &right) {
    return left.Add(right);
  }

  //--------------------------------------------------------------
  Vector2Int operator-(Vector2Int left, const Vector2Int &right) {
    return left.Subtract(right);
  }

  //--------------------------------------------------------------
  Vector2Int operator*(Vector2Int left, const Vector2Int &right) {
    return left.Multiply(right);
  }

  //--------------------------------------------------------------
  Vector2Int operator/(Vector2Int left, const Vector2Int &right) {
    return left.Divide(right);
  }

  //--------------------------------------------------------------
  Vector2Int operator+(Vector2Int left, int32 right) {
    return left.Add(right);
  }

  //--------------------------------------------------------------
  Vector2Int operator-(Vector2Int left, int32 right) {
    return left.Subtract(right);
  }

  //--------------------------------------------------------------
  Vector2Int operator*(Vector2Int left, int32 right) {
    return left.Multiply(right);
  }

  //--------------------------------------------------------------
  Vector2Int operator/(Vector2Int left, int32 right) {
    return left.Divide(right);
  }

  //--------------------------------------------------------------
  Vector2Int operator+(int32 left, Vector2Int right) {
    return right.Add(left);
  }

  //--------------------------------------------------------------
  Vector2Int operator-(int32 left, Vector2Int right) {
    return right.Subtract(left);
  }

  //--------------------------------------------------------------
  Vector2Int operator*(int32 left, Vector2Int right) {
    return right.Multiply(left);
  }

  //--------------------------------------------------------------
  Vector2Int operator/(int32 left, Vector2Int right) {
    return right.Divide(left);
  }

}