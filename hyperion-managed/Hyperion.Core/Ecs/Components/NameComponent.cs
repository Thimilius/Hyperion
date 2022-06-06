using System.Runtime.InteropServices;

namespace Hyperion.Ecs {
  public class NameComponent : Component {
    public unsafe string Name {
      get => Marshal.PtrToStringUTF8(Bindings.NameComponent.GetName(Entity.EntityManager.NativeHandle, Entity.Id));
      set => Bindings.NameComponent.SetName(Entity.EntityManager.NativeHandle, Entity.Id, value);
    }
  }
}