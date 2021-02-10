using System;
using System.Runtime.CompilerServices;

namespace Hyperion {
    public abstract class Component : Object {
        public Entity Entity => Binding_GetEntity(this);
        public Transform Transform => Entity.Transform;

        public T GetComponent<T>() where T : Component => Entity.GetComponent<T>();
        public Component GetComponent(Type type) => Entity.GetComponent(type);

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Entity Binding_GetEntity(Component @this);
    }
}
