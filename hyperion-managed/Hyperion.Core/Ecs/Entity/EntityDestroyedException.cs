using System;

namespace Hyperion.Ecs {
  public class EntityDestroyedException : Exception {
    public EntityDestroyedException() : base("Trying to access a destroyed entity!") { }
  }
}