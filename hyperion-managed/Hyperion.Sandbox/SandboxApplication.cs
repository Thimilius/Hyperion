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
    }

    public void Update() {
      Engine.Log(m_Entity.GetComponent<NameComponent>().Name);
    }

    public void Shutdown() {
      
    }
  }
}