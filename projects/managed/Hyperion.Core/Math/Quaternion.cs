using System;
using System.Runtime.CompilerServices;

namespace Hyperion {
    public struct Quaternion : IEquatable<Quaternion> {
        public float x;
        public float y;
        public float z;
        public float w;

        public static Quaternion Identity => new Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

        public Quaternion(float x, float y, float z, float w) {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public override string ToString() {
            return $"({x:0.00}, {y:0.00}, {z:0.00}, {w:0.00})";
        }

        public override bool Equals(object obj) {
            if (obj is Quaternion other) {
                return this == other;
            } else {
                return false;
            }
        }

        public override int GetHashCode() {
            return x.GetHashCode() ^ y.GetHashCode() << 2 ^ z.GetHashCode() >> 2 ^ w.GetHashCode() >> 1;
        }

        public bool Equals(Quaternion other) {
            return this == other;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator ==(Quaternion lhs, Quaternion rhs) {
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator !=(Quaternion lhs, Quaternion rhs) {
            return !(lhs == rhs);
        }
    }
}
