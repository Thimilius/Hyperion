using System.Numerics;

namespace Hyperion.Ecs {
  public class DerivedTransformComponent : Component {
    public unsafe Vector3 Position {
      get {
        Bindings.DerivedTransformComponent.GetPosition(Entity.EntityManager.NativeHandle, Entity.Id, out Vector3 position);
        return position;
      }
      set => Bindings.DerivedTransformComponent.SetPosition(Entity.EntityManager.NativeHandle, Entity.Id, ref value);
    }
    
    public unsafe Quaternion Rotation {
      get {
        Bindings.DerivedTransformComponent.GetRotation(Entity.EntityManager.NativeHandle, Entity.Id, out Quaternion rotation);
        return rotation;
      }
      set => Bindings.DerivedTransformComponent.SetRotation(Entity.EntityManager.NativeHandle, Entity.Id, ref value);
    }
    
    public unsafe Vector3 Scale {
      get {
        Bindings.DerivedTransformComponent.GetScale(Entity.EntityManager.NativeHandle, Entity.Id, out Vector3 position);
        return position;
      }
      set => Bindings.DerivedTransformComponent.SetScale(Entity.EntityManager.NativeHandle, Entity.Id, ref value);
    }
  }
}