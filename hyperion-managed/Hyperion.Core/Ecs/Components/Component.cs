namespace Hyperion.Ecs {
  public abstract class Component {
    public Entity Entity { get; internal init; }
  }
}