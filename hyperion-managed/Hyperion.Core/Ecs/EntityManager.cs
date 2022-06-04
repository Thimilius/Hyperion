namespace Hyperion.Ecs {
  public class EntityManager : Object {
    public unsafe int EntityCount => Bindings.EntityManager.GetEntityCount(NativeHandle);
  }
}