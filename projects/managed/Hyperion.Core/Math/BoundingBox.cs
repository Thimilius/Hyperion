using System;
using System.Runtime.CompilerServices;

namespace Hyperion {
    public struct BoundingBox : IEquatable<BoundingBox> {
        public Vector3 Size => new Vector3(Math.Abs(Max.X - Min.X), Math.Abs(Max.Y - Min.Y), Math.Abs(Max.Z - Min.Z));
        public Vector3 Center => (Max + Min) * 0.5f;

        public Vector3 Min;
        public Vector3 Max;

        public BoundingBox(Vector3 min, Vector3 max) {
            Min = min;
            Max = max;
        }

        public override string ToString() {
            return $"(Min: {Min}, Max: {Max})";
        }

        public override bool Equals(object obj) {
            if (obj is BoundingBox other) {
                return this == other;
            } else {
                return false;
            }
        }

        public override int GetHashCode() {
            return Min.GetHashCode() ^ Max.GetHashCode() << 2;
        }

        public bool Equals(BoundingBox other) {
            return this == other;
        }

        public bool Intersects(BoundingBox other) {
            return (Max > other.Min && Min < other.Max) || (Min > other.Max && Max < other.Min);
        }

        public bool Contains(Vector3 point) {
            return point > Min && point > Max;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator ==(BoundingBox lhs, BoundingBox rhs) {
            return lhs.Min == rhs.Min && lhs.Max == rhs.Max;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator !=(BoundingBox lhs, BoundingBox rhs) {
            return !(lhs == rhs);
        }
    }
}
