namespace Hyperion.Ecs {
  public class EntityManager : Object {
    public unsafe int EntityCount => Bindings.EntityManager.GetEntityCount(NativeHandle);

    private EntityManager() { }
    
    public unsafe EntityId CreateEntity() {
      Bindings.EntityManager.CreateEntity(NativeHandle, out EntityId id);
      return id;
    }
  }
}