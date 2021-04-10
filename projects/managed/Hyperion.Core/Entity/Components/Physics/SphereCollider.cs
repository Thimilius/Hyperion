using System.Runtime.CompilerServices;

namespace Hyperion {
    public sealed class SphereCollider : Collider {
        public override ColliderType ColliderType => ColliderType.SphereCollider;

        public float Radius {
            get {
                Binding_GetRadius(this, out float radius);
                return radius;
            }
            set => Binding_SetRadius(this, ref value);
        }

        private SphereCollider() { }

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_GetRadius(SphereCollider @this, out float radius);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_SetRadius(SphereCollider @this, ref float radius);
    }
}
