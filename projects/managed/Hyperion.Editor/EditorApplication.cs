namespace Hyperion.Editor {
    public class EditorApplication : IApplication {
        private class MyComponent : Script {

        }

        private Entity m_Entity;

        public void Initialize() {
            m_Entity = Entity.CreatePrimitive(EntityPrimitive.Sphere);
        }

        public void Update(float deltaTime) {
            Vector3 position = m_Entity.Transform.Position;
            position.x += deltaTime;
            m_Entity.Transform.Position = position;

            m_Entity.AddComponent<MyComponent>();

            if (Input.IsKeyDown(KeyCode.Delete)) {
                Object.Destroy(m_Entity);
            }
        }

        public void FixedUpdate(float fixedDeltaTime) {

        }

        public void Shutdown() {

        }
    }
}
