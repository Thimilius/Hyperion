using System;
using System.Runtime.CompilerServices;

namespace Hyperion {
    public struct Vector4 : IEquatable<Vector4> {
        public float Magnitude => (float)Math.Sqrt(x * x + y * y + z * z + w * w);
        public float SqrMagnitude => x * x + y * y + z * z + w * w;

        public float x;
        public float y;
        public float z;
        public float w;

        public Vector4(float x, float y, float z, float w) {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public Vector4(Vector2 vector2, float z, float w) {
            x = vector2.x;
            y = vector2.y;
            this.z = z;
            this.w = w;
        }

        public Vector4(Vector3 vector3, float w) {
            x = vector3.x;
            y = vector3.y;
            z = vector3.z;
            this.w = w;
        }

        public override string ToString() {
            return $"({x:0.00}, {y:0.00}, {z:0.00}, {w:0.00})";
        }

        public override bool Equals(object obj) {
            if (obj is Vector4 other) {
                return this == other;
            } else {
                return false;
            }
        }

        public override int GetHashCode() {
            return base.GetHashCode();
        }

        public bool Equals(Vector4 other) {
            return this == other;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator ==(Vector4 lhs, Vector4 rhs) {
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator !=(Vector4 lhs, Vector4 rhs) {
            return !(lhs == rhs);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator +(Vector4 lhs, Vector4 rhs) {
            return new Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator -(Vector4 lhs, Vector4 rhs) {
            return new Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator *(Vector4 lhs, Vector4 rhs) {
            return new Vector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator /(Vector4 lhs, Vector4 rhs) {
            return new Vector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator -(Vector4 vector) {
            return new Vector4(-vector.x, -vector.y, -vector.z, -vector.w);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator *(Vector4 lhs, float rhs) {
            return new Vector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator *(float lhs, Vector4 rhs) {
            return new Vector4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator /(Vector4 lhs, float rhs) {
            return new Vector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
        }
    }
}
