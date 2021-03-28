namespace Hyperion.Editor {
    public class EditorApplication : IApplication {
        public void Initialize() {
            Engine.Log("Initialize");
        }

        public void Update(float deltaTime) {
            Engine.Log("Update {0}", Time.ElapsedTime);
        }

        public void FixedUpdate(float fixedDeltaTime) {

        }

        public void Shutdown() {
            Engine.Log("Shutdown");
        }
    }
}
