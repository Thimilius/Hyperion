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
      
      NameComponent nameComponent = m_Entity.GetComponent<NameComponent>();
      nameComponent.Name = "Hello there!";
      Engine.Log(nameComponent.Name);

      NameComponent secondComponent = m_Entity.GetComponent<NameComponent>();
      Engine.Log(nameComponent == secondComponent);
    }

    public void Update() {
      
    }

    public void Shutdown() {
      
    }
  }
}