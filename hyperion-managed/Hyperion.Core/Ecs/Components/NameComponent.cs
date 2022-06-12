using System.Runtime.InteropServices;

namespace Hyperion.Ecs {
  public class NameComponent : Component {
    public unsafe string Name {
      get {
        ValidateIsAlive();
        return Marshal.PtrToStringUTF8(Bindings.NameComponent.GetName(Entity.EntityManager.NativeHandle, Entity.Id));
      }
      set {
        ValidateIsAlive();
        Bindings.NameComponent.SetName(Entity.EntityManager.NativeHandle, Entity.Id, value);
      }
    }
  }
}