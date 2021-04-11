using System;
using System.Runtime.CompilerServices;

namespace Hyperion {
    public struct Plane : IEquatable<Plane> {
        public Vector3 Normal;
        public float Distance;

        public Plane(Vector3 normal, float distance) {
            Normal = normal;
            Distance = distance;
        }

        public Plane(Vector3 normal, Vector3 point) {
            Normal = normal.Normalized;
            Distance = -Vector3.Dot(Normal, point);
        }

        public Plane(Vector3 a, Vector3 b, Vector3 c) {
            Normal = Vector3.Cross(b - a, c - a).Normalized;
            Distance = -Vector3.Dot(Normal, a);
        }

        public void Flip() {
            Normal = -Normal;
            Distance = -Distance;
        }

        public Vector3 GetClosestPoint(Vector3 point) {
            float d = Vector3.Dot(Normal, point) + Distance;
            return point - Normal * d;
        }

        public float GetDistanceToPoint(Vector3 point) {
            return Vector3.Dot(Normal, point) + Distance;
        }

        public bool Intersects(Ray ray, out float distance) {
            float angle = Vector3.Dot(ray.Direction, Normal);
            float originToPlane = -Vector3.Dot(ray.Origin, Normal) - Distance;
            bool hit;
            if (angle == 0.0f) {
                distance = 0.0f;
                hit = false;
            } else {
                float dis = originToPlane / angle;
                distance = dis;
                hit = dis > 0.0f;
            }
            return hit;
        }

        public override string ToString() {
            return $"(Normal: {Normal}, Distance: {Distance})";
        }

        public override bool Equals(object obj) {
            if (obj is Plane other) {
                return this == other;
            } else {
                return false;
            }
        }

        public override int GetHashCode() {
            return Normal.GetHashCode() ^ Distance.GetHashCode() << 2;
        }

        public bool Equals(Plane other) {
            return this == other;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator ==(Plane lhs, Plane rhs) {
            return lhs.Normal == rhs.Normal && lhs.Distance == rhs.Distance;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool operator !=(Plane lhs, Plane rhs) {
            return !(lhs == rhs);
        }
    }
}
