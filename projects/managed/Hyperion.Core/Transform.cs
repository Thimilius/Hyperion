using System.Runtime.CompilerServices;

namespace Hyperion {
    public class Transform : Component {
        public Vector3 Position {
            get {
                Binding_GetPosition(this, out Vector3 result);
                return result;
            }
            set {
                Binding_SetPosition(this, ref value);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Entity Binding_GetPosition(Transform transform, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Entity Binding_SetPosition(Transform transform, ref Vector3 position);
    }
}
