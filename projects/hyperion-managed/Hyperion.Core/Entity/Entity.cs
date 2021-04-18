using System;
using System.Runtime.CompilerServices;

namespace Hyperion {
    public enum EntityPrimitive {
        Quad,
        Plane,
        Cube,
        Sphere,

        DirectionalLight,
        PointLight,
        SpotLight,

        Camera
    }

    public class Entity : Object {
        public Transform Transform => Binding_GetTransform(this);
        public World World => Binding_GetWorld(this);

        public Entity() => Binding_Ctor(this, null);
        public Entity(string name) => Binding_Ctor(this, name);

        public T AddComponent<T>() where T : Component => (T)AddComponent(typeof(T));
        public Component AddComponent(Type type) => Binding_AddComponent(this, type);

        public T GetComponent<T>() where T : Component => (T)GetComponent(typeof(T));
        public Component GetComponent(Type type) => Binding_GetComponent(this, type);

        public static Entity CreatePrimitive(EntityPrimitive primitive) => Binding_CreatePrimitive(primitive);

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Transform Binding_GetTransform(Entity entity);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern World Binding_GetWorld(Entity entity);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Entity Binding_Ctor(Entity entity, string name);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Component Binding_AddComponent(Entity entity, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Component Binding_GetComponent(Entity entity, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Entity Binding_CreatePrimitive(EntityPrimitive primitive);
    }
}
