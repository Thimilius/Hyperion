using System.Numerics;

namespace Hyperion.Ecs.Components {
  public struct LocalTransformComponent {
    public Vector3 Position;
    public Quaternion Rotation;
    public Vector3 Scale;
  }
}