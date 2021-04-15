using System;
using System.Runtime.CompilerServices;

namespace Hyperion {
    public struct Color : IEquatable<Color> {
        public static Color Black => new Color(0.0f, 0.0f, 0.0f, 1.0f);
        public static Color Red => new Color(1.0f, 0.0f, 0.0f, 1.0f);
        public static Color Green => new Color(0.0f, 1.0f, 0.0f, 1.0f);
        public static Color Blue => new Color(0.0f, 0.0f, 1.0f, 1.0f);
        public static Color Yellow => new Color(1.0f, 1.0f, 0.0f, 1.0f);
        public static Color Magenta => new Color(1.0f, 0.0f, 1.0f, 1.0f);
        public static Color Cyan => new Color(0.0f, 1.0f, 1.0f, 1.0f);
        public static Color Grey => new Color(0.5f, 0.5f, 0.5f, 1.0f);
        public static Color White => new Color(1.0f, 1.0f, 1.0f, 1.0f);
        public static Color Transparent => new Color(0.0f, 0.0f, 0.0f, 0.0f);

        public float R;
        public float G;
        public float B;
        public float A;

        public Color(float r, float g, float b, float a) {
            R = r;
            G = g;
            B = b;
            A = a;
        }

        public Color(Vector4 vector4) {
            R = vector4.X;
            G = vector4.Y;
            B = vector4.Z;
            A = vector4.W;
        }

        public override string ToString() {
            return $"({R:0.00}, {G:0.00}, {B:0.00}, {A:0.00})";
        }

        public override bool Equals(object obj) {
            if (obj is Color other) {
                return this == other;
            } else {
                return false;
            }
        }

        public override int GetHashCode() {
            return R.GetHashCode() ^ G.GetHashCode() << 2 ^ B.GetHashCode() >> 2 ^ A.GetHashCode() >> 1;
        }

        public bool Equals(Color other) {
            return this == other;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator ==(Color lhs, Color rhs) {
            return lhs.R == rhs.R && lhs.G == rhs.G && lhs.B == rhs.B && lhs.A == rhs.A;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator !=(Color lhs, Color rhs) {
            return !(lhs == rhs);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color operator +(Color lhs, Color rhs) {
            return new Color(lhs.R + rhs.R, lhs.G + rhs.G, lhs.B + rhs.B, lhs.A + rhs.A);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color operator -(Color lhs, Color rhs) {
            return new Color(lhs.R - rhs.R, lhs.G - rhs.G, lhs.B - rhs.B, lhs.A - rhs.A);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color operator *(Color lhs, Color rhs) {
            return new Color(lhs.R * rhs.R, lhs.G * rhs.G, lhs.B * rhs.B, lhs.A * rhs.A);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color operator /(Color lhs, Color rhs) {
            return new Color(lhs.R / rhs.R, lhs.G / rhs.G, lhs.B / rhs.B, lhs.A / rhs.A);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color operator -(Color vector) {
            return new Color(-vector.R, -vector.G, -vector.B, -vector.A);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color operator *(Color lhs, float rhs) {
            return new Color(lhs.R * rhs, lhs.G * rhs, lhs.B * rhs, lhs.A * rhs);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color operator *(float lhs, Color rhs) {
            return new Color(lhs * rhs.R, lhs * rhs.G, lhs * rhs.B, lhs * rhs.A);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Color operator /(Color lhs, float rhs) {
            return new Color(lhs.R / rhs, lhs.G / rhs, lhs.B / rhs, lhs.A / rhs);
        }

        public static implicit operator Color(Vector4 vector4) => new Color(vector4);
    }
}
