using System;

namespace Hyperion.Ecs {
  public class ComponentDestroyedException : Exception {
    public ComponentDestroyedException() : base("Trying to access a destroyed component!") { }
  }
}