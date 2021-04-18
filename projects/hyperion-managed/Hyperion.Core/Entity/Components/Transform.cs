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

        public Quaternion Rotation {
            get {
                Binding_GetRotation(this, out Quaternion result);
                return result;
            }
            set {
                Binding_SetRotation(this, ref value);
            }
        }

        public Vector3 EulerAngles {
            get {
                Binding_GetEulerAngles(this, out Vector3 result);
                return result;
            }
            set {
                Binding_SetEulerAngles(this, ref value);
            }
        }

        public Transform Parent {
            get => Binding_GetParent(this);
            set => Binding_SetParent(this, value);
        }

        protected Transform() { }

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_GetPosition(Transform @this, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_SetPosition(Transform @this, ref Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_GetRotation(Transform @this, out Quaternion position);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_SetRotation(Transform @this, ref Quaternion position);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_GetEulerAngles(Transform @this, out Vector3 eulerAngles);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_SetEulerAngles(Transform @this, ref Vector3 eulerAngles);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Transform Binding_GetParent(Transform @this);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_SetParent(Transform @this, Transform parent);
    }
}
