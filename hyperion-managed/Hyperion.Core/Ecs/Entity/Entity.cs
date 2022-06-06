using System;
using System.Collections.Generic;

namespace Hyperion.Ecs {
  public class Entity {
    // TODO: Here is a hard problem we need to solve:
    // How do we update the cache to not contain removed components anymore?
    // This does not only need to work from the managed side (which is easy)
    // but also when just doing stuff from the native side.
    private readonly Dictionary<Type, Component> m_ComponentCached = new();

    public EntityManager EntityManager { get; }
    public EntityId Id { get; }

    internal Entity(EntityManager entityManager, EntityId id) {
      EntityManager = entityManager;
      Id = id;
    }
    
    public T GetComponent<T>() where T : Component, new() {
      Type componentType = typeof(T);

      bool inCache = m_ComponentCached.TryGetValue(componentType, out Component component);
      if (inCache) {
        return component as T;
      }
      
      // TODO: Check for existence first.
      
      // TODO: This should probably be done by native code somehow.
      T newComponent = new() {
        Entity = this
      };
      m_ComponentCached[componentType] = newComponent;
      return newComponent;
    }
  }
}