using System;
using System.Runtime.CompilerServices;

namespace Hyperion {
    public struct Vector4 : IEquatable<Vector4> {
        public float Magnitude => (float)Math.Sqrt(X * X + Y * Y + Z * Z + W * W);
        public float SqrMagnitude => X * X + Y * Y + Z * Z + W * W;

        public float X;
        public float Y;
        public float Z;
        public float W;

        public Vector4(float x, float y, float z, float w) {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Vector4(Vector2 vector2, float z, float w) {
            X = vector2.X;
            Y = vector2.Y;
            Z = z;
            W = w;
        }

        public Vector4(Vector3 vector3, float w) {
            X = vector3.X;
            Y = vector3.Y;
            Z = vector3.Z;
            W = w;
        }

        public override string ToString() {
            return $"({X:0.00}, {Y:0.00}, {Z:0.00}, {W:0.00})";
        }

        public override bool Equals(object obj) {
            if (obj is Vector4 other) {
                return this == other;
            } else {
                return false;
            }
        }

        public override int GetHashCode() {
            return X.GetHashCode() ^ Y.GetHashCode() << 2 ^ Z.GetHashCode() >> 2 ^ W.GetHashCode() >> 1;
        }

        public bool Equals(Vector4 other) {
            return this == other;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator ==(Vector4 lhs, Vector4 rhs) {
            return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z && lhs.W == rhs.W;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator !=(Vector4 lhs, Vector4 rhs) {
            return !(lhs == rhs);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator <(Vector4 lhs, Vector4 rhs) {
            return lhs.X < rhs.X && lhs.Y < rhs.Y && lhs.Z < rhs.Z && lhs.W < rhs.W;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator >(Vector4 lhs, Vector4 rhs) {
            return lhs.X > rhs.X && lhs.Y > rhs.Y && lhs.Z > rhs.Z && lhs.W < rhs.W;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator +(Vector4 lhs, Vector4 rhs) {
            return new Vector4(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z, lhs.W + rhs.W);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator -(Vector4 lhs, Vector4 rhs) {
            return new Vector4(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z, lhs.W - rhs.W);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator *(Vector4 lhs, Vector4 rhs) {
            return new Vector4(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z, lhs.W * rhs.W);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator /(Vector4 lhs, Vector4 rhs) {
            return new Vector4(lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z, lhs.W / rhs.W);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator -(Vector4 vector) {
            return new Vector4(-vector.X, -vector.Y, -vector.Z, -vector.W);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator *(Vector4 lhs, float rhs) {
            return new Vector4(lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs, lhs.W * rhs);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator *(float lhs, Vector4 rhs) {
            return new Vector4(lhs * rhs.X, lhs * rhs.Y, lhs * rhs.Z, lhs * rhs.W);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector4 operator /(Vector4 lhs, float rhs) {
            return new Vector4(lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs, lhs.W / rhs);
        }
    }
}
