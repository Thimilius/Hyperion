namespace Hyperion.Sandbox {
  public class SandboxApplication : IApplication {
    public void Initialize() {
      Engine.Log("Initialize");
    }

    public void Update() {
      Engine.Log("Update");
    }

    public void Shutdown() {
      Engine.Log("Shutdown");
    }
  }
}