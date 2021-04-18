using System.Runtime.CompilerServices;

namespace Hyperion {
    public enum ColliderType {
        BoxCollider,
        SphereCollider,
    }

    public abstract class Collider : Behaviour {
        public abstract ColliderType ColliderType { get; }

        public BoundingBox BoundingBox {
            get {
                Binding_GetBoundingBox(this, out BoundingBox boundingBox);
                return boundingBox;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_GetBoundingBox(Collider @this, out BoundingBox boundingBox);
    }
}
