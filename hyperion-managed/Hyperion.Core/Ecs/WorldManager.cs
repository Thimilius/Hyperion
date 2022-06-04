using System.Runtime.InteropServices;

namespace Hyperion.Ecs {
  public static class WorldManager {
    public static unsafe World GetActiveWorld() {
      GCHandle handle = GCHandle.FromIntPtr(Bindings.WorldManager.GetActiveWorld());
      return handle.Target as World;
    }
  }
}