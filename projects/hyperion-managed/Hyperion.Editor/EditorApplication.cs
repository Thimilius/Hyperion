namespace Hyperion.Editor {
    public class EditorApplication : IApplication {
        private class MyComponent : Script {
            public int Value { get; set; }

            protected override void OnCreate() {
                Engine.Log("OnCreate");
            }
            
            protected override void OnDestroy() {
                Engine.Log("OnDestroy");
            }
        }

        private Entity m_Entity;

        public void Initialize() {
            m_Entity = Entity.CreatePrimitive(EntityPrimitive.Sphere);
        }

        public void Update(float deltaTime) {
            Vector3 position = m_Entity.Transform.Position;
            position.X += deltaTime;
            m_Entity.Transform.Position = position;
            
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
