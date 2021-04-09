using System;
using System.Runtime.CompilerServices;

namespace Hyperion {
    public struct Vector2 : IEquatable<Vector2> {
        public float Magnitude => (float)Math.Sqrt(X * X + Y * Y);
        public float SqrMagnitude => X * X + Y * Y;

        public float X;
        public float Y;

        public Vector2(float x, float y) {
            X = x;
            Y = y;
        }

        public override string ToString() {
            return $"({X:0.00}, {Y:0.00})";
        }

        public override bool Equals(object obj) {
            if (obj is Vector2 other) {
                return this == other;
            } else {
                return false;
            }
        }

        public override int GetHashCode() {
            return X.GetHashCode() ^ Y.GetHashCode() << 2;
        }

        public bool Equals(Vector2 other) {
            return this == other;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Dot(Vector2 lhs, Vector2 rhs) {
            return lhs.X * rhs.X + lhs.Y * rhs.Y;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator ==(Vector2 lhs, Vector2 rhs) {
            return lhs.X == rhs.X && lhs.Y == rhs.Y;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator !=(Vector2 lhs, Vector2 rhs) {
            return !(lhs == rhs);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator <(Vector2 lhs, Vector2 rhs) {
            return lhs.X < rhs.X && lhs.Y < rhs.Y;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator >(Vector2 lhs, Vector2 rhs) {
            return lhs.X > rhs.X && lhs.Y > rhs.Y;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 operator +(Vector2 lhs, Vector2 rhs) {
            return new Vector2(lhs.X + rhs.X, lhs.Y + rhs.Y);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 operator -(Vector2 lhs, Vector2 rhs) {
            return new Vector2(lhs.X - rhs.X, lhs.Y - rhs.Y);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 operator *(Vector2 lhs, Vector2 rhs) {
            return new Vector2(lhs.X * rhs.X, lhs.Y * rhs.Y);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 operator /(Vector2 lhs, Vector2 rhs) {
            return new Vector2(lhs.X / rhs.X, lhs.Y / rhs.Y);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 operator -(Vector2 vector) {
            return new Vector2(-vector.X, -vector.Y);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 operator *(Vector2 lhs, float rhs) {
            return new Vector2(lhs.X * rhs, lhs.Y * rhs);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 operator *(float lhs, Vector2 rhs) {
            return new Vector2(lhs * rhs.X, lhs * rhs.Y);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector2 operator /(Vector2 lhs, float rhs) {
            return new Vector2(lhs.X / rhs, lhs.Y / rhs);
        }
    }
}
