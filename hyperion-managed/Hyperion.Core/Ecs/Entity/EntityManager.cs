using System;
using System.Collections.Generic;

namespace Hyperion.Ecs {
  public sealed class EntityManager : Object {
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

    public unsafe void DestroyEntity(Entity entity) {
      Bindings.EntityManager.DestroyEntity(NativeHandle, entity.Id);
    }
    
    public unsafe bool HasComponent<T>(EntityId id) where T : Component {
      return Bindings.EntityManager.HasComponent(NativeHandle, Native.GetTypeHandle<T>(), id);
    }

    public unsafe void RemoveComponent<T>(EntityId id) where T : Component {
      Bindings.EntityManager.RemoveComponent(NativeHandle, Native.GetTypeHandle<T>(), id);
    }

    internal void OnEntityDestroyedNative(EntityId id) {
      Entity entity = GetEntityById(id);
      if (entity == null) {
        return;
      }
      
      entity.Destroy();
      m_EntityCache.Remove(entity.Id);
    }
    
    internal void OnComponentRemovedNative(Type componentType, EntityId id) {
      Entity entity = GetEntityById(id);
      entity?.OnComponentRemovedNative(componentType);
    }
  }
}