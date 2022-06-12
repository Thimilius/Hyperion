using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace Hyperion.Ecs {
  public sealed class Entity {
    private readonly Dictionary<Type, Component> m_ComponentCached = new();

    public EntityManager EntityManager { get; }
    public EntityId Id { get; }

    public bool IsAlive { get; private set; }
    
    internal Entity(EntityManager entityManager, EntityId id) {
      EntityManager = entityManager;
      Id = id;

      IsAlive = true;
    }

    public bool HasComponent<T>() where T : Component {
      ValidateIsAlive();
      
      return EntityManager.HasComponent<T>(Id);
    }
    
    public T GetComponent<T>() where T : Component, new() {
      ValidateIsAlive();
      
      Type componentType = typeof(T);

      bool inCache = m_ComponentCached.TryGetValue(componentType, out Component component);
      if (inCache) {
        return component as T;
      }

      if (!HasComponent<T>()) {
        return null;
      }
      
      // TODO: This should probably be done by native code somehow.
      T newComponent = new() {
        Entity = this,
        IsAlive = true
      };
      m_ComponentCached[componentType] = newComponent;
      return newComponent;
    }

    public void RemoveComponent<T>() where T : Component {
      ValidateIsAlive();
      
      Type componentType = typeof(T);
      
      if (!m_ComponentCached.ContainsKey(componentType)) {
        return;
      }

      
      Component component = m_ComponentCached[componentType];
      if (!component.IsAlive) {
        return;
      }
      
      EntityManager.RemoveComponent<T>(Id);
    }

    internal void Destroy() {
      IsAlive = false;
      
      foreach (Component component in m_ComponentCached.Values) {
        component.Destroy();
      }
      m_ComponentCached.Clear();
    }

    internal void OnComponentRemovedNative(Type componentType) {
      Component component = m_ComponentCached[componentType];
      component.Destroy();
      m_ComponentCached.Remove(componentType);
    }
    
    [StackTraceHidden]
    private void ValidateIsAlive() {
      if (!IsAlive) {
        throw new EntityDestroyedException();
      }
    }
  }
}