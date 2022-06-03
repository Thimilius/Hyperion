using System.Runtime.InteropServices;

namespace Hyperion {
  public static class WorldManager {
    public static unsafe World GetActiveWorld() {
      var worldHandle = GCHandle.FromIntPtr(Bindings.WorldManager.GetActiveWorld());
      return worldHandle.Target as World;
    }
  }
}