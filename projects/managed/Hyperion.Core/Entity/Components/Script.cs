namespace Hyperion {
    public abstract class Script : Component {
        protected virtual void OnCreate() { }
        protected virtual void OnUpdate(float deltaTime) { }
        protected virtual void OnDestroy() { }
    }
}
