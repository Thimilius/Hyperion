using System.Runtime.CompilerServices;

namespace Hyperion {
    public abstract class Behaviour : Component {
        public bool IsActiveAndEnabled {
            get {
                Binding_IsActiveAndEnabled(this, out bool value);
                return value;
            }
        }
        public bool IsEnabled {
            get {
                Binding_IsEnabled(this, out bool value);
                return value;
            }
            set => Binding_SetEnabled(this, ref value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_IsActiveAndEnabled(Behaviour @this, out bool isActiveAndEnabled);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_IsEnabled(Behaviour @this, out bool isEnabled);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_SetEnabled(Behaviour @this, ref bool isEnabled);
    }
}
