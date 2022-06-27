using System.Numerics;
using Hyperion.Ecs;

namespace Hyperion.Sandbox {
  public class EditorApplication : IApplication {
    private World m_World;
    private Entity m_Entity;
    
    public void Initialize() {
      m_World = WorldManager.GetActiveWorld();
      EntityManager entityManager = m_World.EntityManager;
      
      m_Entity = entityManager.CreateEntity(EntityPrimitive.Cube);
      
      Engine.Log("Hello there from C#");
    }

    public void Update() {
      LocalTransformComponent localTransformComponent = m_Entity.GetComponent<LocalTransformComponent>();
      Vector3 position = localTransformComponent.Position;

      const float speed = 10.0f;
      if (Input.IsKeyHold(KeyCode.Up)) {
        position.Z -= speed * Time.DeltaTime;
      }
      if (Input.IsKeyHold(KeyCode.Down)) {
        position.Z += speed * Time.DeltaTime;
      }
      if (Input.IsKeyHold(KeyCode.Left)) {
        position.X -= speed * Time.DeltaTime;
      }
      if (Input.IsKeyHold(KeyCode.Right)) {
        position.X += speed * Time.DeltaTime;
      }
      
      localTransformComponent.Position = position;
    }

    public void Shutdown() {
      
    }
  }
}