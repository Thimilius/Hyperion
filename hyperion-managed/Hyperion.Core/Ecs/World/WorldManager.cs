using System.Runtime.InteropServices;

namespace Hyperion.Ecs.World {
  public static class WorldManager {
    public static unsafe World GetActiveWorld() {
      return GCHandle.FromIntPtr(Bindings.WorldManager.GetActiveWorld()).Get<World>();
    }
  }
}