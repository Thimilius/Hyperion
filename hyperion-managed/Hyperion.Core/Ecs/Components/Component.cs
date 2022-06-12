using System.Diagnostics;

namespace Hyperion.Ecs {
  public abstract class Component {
    public Entity Entity { get; internal init; }
    public bool IsAlive { get; internal set; }

    internal void Destroy() {
      IsAlive = false;
    }

    [StackTraceHidden]
    protected void ValidateIsAlive() {
      if (!IsAlive) {
        throw new ComponentDestroyedException();
      }
    } 
  }
}