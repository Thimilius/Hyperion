namespace Hyperion.Editor {
    public class EditorApplication : IApplication {
        public void Initialize() {
            Engine.Log("Managed Initialize");
        }

        public void Update(float deltaTime) {
            Engine.Log("Managed Update {0}", deltaTime);
        }

        public void FixedUpdate(float deltaTime) {
            Engine.Log("Managed Fixed Update {0}", deltaTime);
        }

        public void Shutdown() {
            Engine.Log("Managed Shutdown");
        }
    }
}
