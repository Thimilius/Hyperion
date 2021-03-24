namespace Hyperion {
    public interface IApplication {
        void Initialize();
        void Update(float deltaTime);
        void FixedUpdate(float deltaTime);
        void Shutdown();
    }
}
