using System.Diagnostics;
using Hyperion.Ecs;

namespace Hyperion.Sandbox {
  public class SandboxApplication : IApplication {
    private World m_World;
    private Entity m_Entity;
    
    public void Initialize() {
      m_World = WorldManager.GetActiveWorld();
      EntityManager entityManager = m_World.EntityManager;
      
      m_Entity = entityManager.CreateEntity();
      
      Engine.Log(m_Entity.HasComponent<NameComponent>());
      
      NameComponent nameComponent = m_Entity.GetComponent<NameComponent>();
      nameComponent.Name = "Hello there!";
      Engine.Log(nameComponent.Name);

      NameComponent secondComponent = m_Entity.GetComponent<NameComponent>();
      Engine.Log(nameComponent == secondComponent);

      m_Entity.RemoveComponent<NameComponent>();
      Engine.Log(nameComponent.Name);
      
      entityManager.DestroyEntity(m_Entity);
      
      Engine.Log(m_Entity.IsAlive);
      Engine.Log(nameComponent.Name);
    }

    public void Update() {
      
    }

    public void Shutdown() {
      
    }
  }
}