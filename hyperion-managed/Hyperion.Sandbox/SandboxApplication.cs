using Hyperion.Ecs;

namespace Hyperion.Sandbox {
  public class SandboxApplication : IApplication {
    private World m_World;
    
    public void Initialize() {
      m_World = WorldManager.GetActiveWorld();
      Engine.Log(m_World.EntityManager.EntityCount);
    }

    public void Update() {
      Engine.Log(m_World.Name);
    }

    public void Shutdown() {
      
    }
  }
}