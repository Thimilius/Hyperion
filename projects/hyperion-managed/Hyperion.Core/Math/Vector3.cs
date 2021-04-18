using System;
using System.Runtime.CompilerServices;

namespace Hyperion {
    public struct Vector3 : IEquatable<Vector3> {
        public static Vector3 Zero => new Vector3(0.0f, 0.0f, 0.0f);
        public static Vector3 One => new Vector3(1.0f, 1.0f, 1.0f);
        public static Vector3 Forward => new Vector3(0.0f, 0.0f, -1.0f);
        public static Vector3 Back => new Vector3(0.0f, 0.0f, 1.0f);
        public static Vector3 Up => new Vector3(0.0f, 1.0f, 0.0f);
        public static Vector3 Down => new Vector3(0.0f, -1.0f, 0.0f);
        public static Vector3 Left => new Vector3(-1.0f, 0.0f, 0.0f);
        public static Vector3 Right => new Vector3(1.0f, 0.0f, 0.0f);

        public float Magnitude => (float)Math.Sqrt(X * X + Y * Y + Z * Z);
        public float SqrMagnitude => X * X + Y * Y + Z * Z;
        public Vector3 Normalized {
            get {
                float magnitude = Magnitude;
                if (magnitude == 0) {
                    return Zero;
                } else {
                    return new Vector3(X / magnitude, Y / magnitude, Z / magnitude);
                }
            }
        }

        public float X;
        public float Y;
        public float Z;

        public Vector3(float x, float y) {
            X = x;
            Y = y;
            Z = 0.0f;
        }

        public Vector3(float x, float y, float z) {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector3(Vector2 vector2, float z) {
            X = vector2.X;
            Y = vector2.Y;
            Z = z;
        }

        public override string ToString() {
            return $"({X:0.00}, {Y:0.00}, {Z:0.00})";
        }

        public override bool Equals(object obj) {
            if (obj is Vector3 other) {
                return this == other;
            } else {
                return false;
            }
        }

        public override int GetHashCode() {
            return X.GetHashCode() ^ Y.GetHashCode() << 2 ^ Z.GetHashCode() >> 2;
        }

        public bool Equals(Vector3 other) {
            return this == other;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Dot(Vector3 lhs, Vector3 rhs) {
            return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 Cross(Vector3 lhs, Vector3 rhs) {
            float x = lhs.Y * rhs.Z - lhs.Z * rhs.Y;
            float y = lhs.Z * rhs.X - lhs.X * rhs.Z;
            float z = lhs.X * rhs.Y - lhs.Y * rhs.X;

            return new Vector3(x, y, z);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator ==(Vector3 lhs, Vector3 rhs) {
            return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator !=(Vector3 lhs, Vector3 rhs) {
            return !(lhs == rhs);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator <(Vector3 lhs, Vector3 rhs) {
            return lhs.X < rhs.X && lhs.Y < rhs.Y && lhs.Z < rhs.Z;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator >(Vector3 lhs, Vector3 rhs) {
            return lhs.X > rhs.X && lhs.Y > rhs.Y && lhs.Z > rhs.Z;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator +(Vector3 lhs, Vector3 rhs) {
            return new Vector3(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator -(Vector3 lhs, Vector3 rhs) {
            return new Vector3(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator *(Vector3 lhs, Vector3 rhs) {
            return new Vector3(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator /(Vector3 lhs, Vector3 rhs) {
            return new Vector3(lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator -(Vector3 vector) {
            return new Vector3(-vector.X, -vector.Y, -vector.Z);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator *(Vector3 lhs, float rhs) {
            return new Vector3(lhs.X * rhs, lhs.Y * rhs, lhs.Z * rhs);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator *(float lhs, Vector3 rhs) {
            return new Vector3(lhs * rhs.X, lhs * rhs.Y, lhs * rhs.Z);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Vector3 operator /(Vector3 lhs, float rhs) {
            return new Vector3(lhs.X / rhs, lhs.Y / rhs, lhs.Z / rhs);
        }
    }
}
