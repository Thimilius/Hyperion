using System.Runtime.CompilerServices;

namespace Hyperion {
    public abstract class Component : Object {
        public Entity Entity => Binding_GetEntity(this);

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Entity Binding_GetEntity(Component component);
    }
}
