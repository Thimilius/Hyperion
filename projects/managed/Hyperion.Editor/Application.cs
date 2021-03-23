namespace Hyperion.Editor {
    public class Application : IApplicationCallbacks {
        public void Initialize() {

        }

        public void Update(float deltaTime) {
            Engine.Log("Hello there from managed code! {0}", Time.ElapsedTime);
        }

        public void FixedUpdate(float deltaTime) {

        }

        public void Shutdown() {

        }
    }
}
