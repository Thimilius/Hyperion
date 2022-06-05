using System.Runtime.InteropServices;

namespace Hyperion.Ecs {
  public class World : Object {
    public unsafe string Name {
      get => Marshal.PtrToStringUTF8(Bindings.World.GetName(NativeHandle));
      set => Bindings.World.SetName(NativeHandle, value);
    }

    public unsafe EntityManager EntityManager => GCHandle.FromIntPtr(Bindings.World.GetEntityManager(NativeHandle)).Get<EntityManager>();
  }
}