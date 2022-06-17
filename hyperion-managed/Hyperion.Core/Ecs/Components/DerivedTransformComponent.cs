using System.Numerics;

namespace Hyperion.Ecs {
  public class DerivedTransformComponent : Component {
    public unsafe Vector3 Position {
      get {
        ValidateIsAlive();
        Bindings.DerivedTransformComponent.GetPosition(Entity.EntityManager.NativeHandle, Entity.Id, out Vector3 position);
        return position;
      }
      set {
        ValidateIsAlive();
        Bindings.DerivedTransformComponent.SetPosition(Entity.EntityManager.NativeHandle, Entity.Id, ref value);
      }
    }
    
    public unsafe Quaternion Rotation {
      get {
        ValidateIsAlive();
        Bindings.DerivedTransformComponent.GetRotation(Entity.EntityManager.NativeHandle, Entity.Id, out Quaternion rotation);
        return rotation;
      }
      set {
        ValidateIsAlive();
        Bindings.DerivedTransformComponent.SetRotation(Entity.EntityManager.NativeHandle, Entity.Id, ref value);
      }
    }
    
    public unsafe Vector3 Scale {
      get {
        ValidateIsAlive();
        Bindings.DerivedTransformComponent.GetScale(Entity.EntityManager.NativeHandle, Entity.Id, out Vector3 position);
        return position;
      }
      set {
        ValidateIsAlive();
        Bindings.DerivedTransformComponent.SetScale(Entity.EntityManager.NativeHandle, Entity.Id, ref value);
      }
    }
  }
}