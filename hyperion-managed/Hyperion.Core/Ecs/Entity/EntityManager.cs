using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace Hyperion.Ecs {
  public class EntityManager : Object {
    // TODO: We have the same problem here as the component cache in Entity.
    private readonly Dictionary<EntityId, Entity> m_EntityCache = new();

    public unsafe int EntityCount => Bindings.EntityManager.GetEntityCount(NativeHandle);

    private EntityManager() { }

    public Entity GetEntityById(EntityId id) {
      m_EntityCache.TryGetValue(id, out Entity entity);
      return entity;
    }
    
    public unsafe Entity CreateEntity() {
      Bindings.EntityManager.CreateEntity(NativeHandle, out EntityId id);

      Entity entity = new Entity(this, id);
      m_EntityCache[id] = entity;

      return entity;
    }
    
    public unsafe ref T GetComponent<T>(EntityId id) where T : struct {
      return ref Unsafe.AsRef<T>((void *)Bindings.EntityManager.GetComponent(NativeHandle, Native.GetTypeHandle<T>(), id));
    }
  }
}