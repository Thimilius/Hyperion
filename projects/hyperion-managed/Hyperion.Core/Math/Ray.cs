using System;
using System.Runtime.CompilerServices;

namespace Hyperion {
    public struct Ray : IEquatable<Ray> {
        public Vector3 Origin;
        public Vector3 Direction;

        public Ray(Vector3 origin, Vector3 direction) {
            Origin = origin;
            Direction = direction;
        }

        public Vector3 GetPoint(float t) {
            return Origin + Direction * t;
        }

        public override string ToString() {
            return $"(Origin: {Origin}, Direction: {Direction})";
        }

        public override bool Equals(object obj) {
            if (obj is Ray other) {
                return this == other;
            } else {
                return false;
            }
        }

        public override int GetHashCode() {
            return Origin.GetHashCode() ^ Direction.GetHashCode() << 2;
        }

        public bool Equals(Ray other) {
            return this == other;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator ==(Ray lhs, Ray rhs) {
            return lhs.Origin == rhs.Origin && lhs.Direction == rhs.Direction;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator !=(Ray lhs, Ray rhs) {
            return !(lhs == rhs);
        }
    }
}
