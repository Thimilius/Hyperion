using System.Runtime.InteropServices;

namespace Hyperion {
  public static class WorldManager {
    public static unsafe World GetActiveWorld() {
      var handle = GCHandle.FromIntPtr(Bindings.WorldManager.GetActiveWorld());
      return handle.Target as World;
    }
  }
}