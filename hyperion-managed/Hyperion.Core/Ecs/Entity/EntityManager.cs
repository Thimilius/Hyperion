using System.Runtime.CompilerServices;

namespace Hyperion.Ecs {
  public class EntityManager : Object {
    public unsafe int EntityCount => Bindings.EntityManager.GetEntityCount(NativeHandle);

    private EntityManager() { }
    
    public unsafe EntityId CreateEntity() {
      Bindings.EntityManager.CreateEntity(NativeHandle, out EntityId id);
      return id;
    }

    public unsafe ref T GetComponent<T>(EntityId id) where T : struct {
      return ref Unsafe.AsRef<T>((void *)Bindings.EntityManager.GetComponent(NativeHandle, Native.GetTypeHandle<T>(), id));
    }
  }
}