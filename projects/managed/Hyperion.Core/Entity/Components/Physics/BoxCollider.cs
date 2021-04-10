using System.Runtime.CompilerServices;

namespace Hyperion {
    public sealed class BoxCollider : Collider {
        public override ColliderType ColliderType => ColliderType.BoxCollider;

        public Vector3 Origin {
            get {
                Binding_GetOrigin(this, out Vector3 origin);
                return origin;
            }
            set => Binding_SetOrigin(this, ref value);
        }

        public Vector3 Size {
            get {
                Binding_GetSize(this, out Vector3 size);
                return size;
            }
            set => Binding_SetSize(this, ref value);
        }

        private BoxCollider() { }

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_GetOrigin(BoxCollider @this, out Vector3 origin);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_SetOrigin(BoxCollider @this, ref Vector3 origin);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_GetSize(BoxCollider @this, out Vector3 size);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_SetSize(BoxCollider @this, ref Vector3 size);
    }
}
