using System.Runtime.CompilerServices;

namespace Hyperion {
    public abstract class Renderer : Component {
        public Material Material => Binding_GetMaterial(this);

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern Material Binding_GetMaterial(Renderer renderer);
    }
}
