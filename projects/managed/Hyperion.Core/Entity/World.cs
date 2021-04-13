using System.Runtime.CompilerServices;

namespace Hyperion {
    public sealed class World {
        public string Name {
            get => Binding_GetName(this);
            set => Binding_SetName(this, value);
        }

        public Entity[] RootEntities => Binding_GetRootEntities(this);

        private World() { }

        public override string ToString() {
            return Name;
        }

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern string Binding_GetName(World @this);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_SetName(World @this, string name);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Entity[] Binding_GetRootEntities(World @this);
    }
}
