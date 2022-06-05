using Hyperion.Ecs;

namespace Hyperion.Sandbox {
  public class SandboxApplication : IApplication {
    private World m_World;
    
    public void Initialize() {
      m_World = WorldManager.GetActiveWorld();
      EntityManager entityManager = m_World.EntityManager;
      EntityId entity = entityManager.CreateEntity();
      Engine.Log(entity);
    }

    public void Update() {
      
    }

    public void Shutdown() {
      
    }
  }
}