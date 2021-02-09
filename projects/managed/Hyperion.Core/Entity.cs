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
        public Entity() => Binding_Ctor(this, null);
        public Entity(string name) => Binding_Ctor(this, name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern Entity Binding_Ctor(Entity entity, string name);
    }
}
