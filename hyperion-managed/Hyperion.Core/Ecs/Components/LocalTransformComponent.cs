using System.Numerics;

namespace Hyperion.Ecs {
  public class LocalTransformComponent : Component {
    public unsafe Vector3 Position {
      get {
        ValidateIsAlive();
        Bindings.LocalTransformComponent.GetPosition(Entity.EntityManager.NativeHandle, Entity.Id, out Vector3 position);
        return position;
      }
      set {
        ValidateIsAlive();
        Bindings.LocalTransformComponent.SetPosition(Entity.EntityManager.NativeHandle, Entity.Id, ref value);
      }
    }
    
    public unsafe Quaternion Rotation {
      get {
        ValidateIsAlive();
        Bindings.LocalTransformComponent.GetRotation(Entity.EntityManager.NativeHandle, Entity.Id, out Quaternion rotation);
        return rotation;
      }
      set { 
        ValidateIsAlive();
        Bindings.LocalTransformComponent.SetRotation(Entity.EntityManager.NativeHandle, Entity.Id, ref value);
      }
    }
    
    public unsafe Vector3 Scale {
      get {
        ValidateIsAlive();
        Bindings.LocalTransformComponent.GetScale(Entity.EntityManager.NativeHandle, Entity.Id, out Vector3 position);
        return position;
      }
      set {
        ValidateIsAlive();
        Bindings.LocalTransformComponent.SetScale(Entity.EntityManager.NativeHandle, Entity.Id, ref value);
      }
    }
  }
}