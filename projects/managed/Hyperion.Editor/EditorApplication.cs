namespace Hyperion.Editor {
    public class EditorApplication : IApplication {
        private Entity m_Entity;

        public void Initialize() {
            m_Entity = Entity.CreatePrimitive(EntityPrimitive.Sphere);
        }

        public void Update(float deltaTime) {
            m_Entity.Transform.Position = new Vector3(Time.ElapsedTime, 0.0f, 0.0f);
        }

        public void FixedUpdate(float fixedDeltaTime) {

        }

        public void Shutdown() {

        }
    }
}
