using Hyperion.Ecs;

namespace Hyperion.Sandbox {
  public class SandboxApplication : IApplication {
    private World m_World;
    private EntityId m_Entity;
    
    public void Initialize() {
      m_World = WorldManager.GetActiveWorld();
      EntityManager entityManager = m_World.EntityManager;
      m_Entity = entityManager.CreateEntity();
    }

    public void Update() {
      ref LocalTransformComponent transform = ref m_World.EntityManager.GetComponent<LocalTransformComponent>(m_Entity);
      transform.Position.X = 0;
    }

    public void Shutdown() {
      
    }
  }
}