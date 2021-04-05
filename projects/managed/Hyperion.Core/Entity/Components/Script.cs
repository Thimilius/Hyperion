using System.Runtime.CompilerServices;

namespace Hyperion {
    public abstract class Script : Component {
        protected Script() => Binding_Ctor(this);

        protected virtual void OnCreate() { }
        protected virtual void OnUpdate(float deltaTime) { }
        protected virtual void OnFixedUpdate(float deltaTime) { }
        protected virtual void OnDestroy() { }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Binding_Ctor(Script @this);
    }
}
