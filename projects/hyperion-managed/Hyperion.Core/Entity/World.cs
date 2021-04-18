using System.Runtime.CompilerServices;

namespace Hyperion {
    public struct WorldEnvironment {
        public float AmbientLightIntensity;
        public Color AmbientLightColor;
    }

    public sealed class World {
        public string Name {
            get => Binding_GetName(this);
            set => Binding_SetName(this, value);
        }

        public WorldEnvironment Environment {
            get {
                Binding_GetEnvironment(this, out WorldEnvironment environment);
                return environment;
            }
            set => Binding_SetEnvironment(this, ref value);
        }

        public Entity[] RootEntities => Binding_GetRootEntities(this);

        private World() { }

        public override string ToString() {
            return Name;
        }

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern string Binding_GetName(World @this);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_SetName(World @this, string name);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_GetEnvironment(World @this, out WorldEnvironment environment);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_SetEnvironment(World @this, ref WorldEnvironment environment);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Entity[] Binding_GetRootEntities(World @this);
    }
}
