namespace Hyperion {
    public interface IApplicationCallbacks {
        void Initialize();
        void Update(float deltaTime);
        void FixedUpdate(float deltaTime);
        void Shutdown();
    }
}
