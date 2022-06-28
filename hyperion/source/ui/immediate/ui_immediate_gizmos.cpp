//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/immediate/ui_immediate_gizmos.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/input.hpp"
#include "hyperion/ecs/component/components/utilities/transform_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  //--------------------------------------------------------------
  GizmoManipulation UIImmediateGizmos::Manipulate(
    Rendering::RenderGizmoType type,
    GizmoMode mode,
    EntityManager *entity_manager,
    EntityId entity,
    DerivedTransformComponent *derived_transform,
    LocalTransformComponent *local_transform,
    Ray ray) {
    static Vector3 offset = Vector3();
    static bool8 should_transform_x = false;
    static bool8 should_transform_y = false;
    static bool8 should_transform_z = false;

    bool8 near_x = false;
    bool8 near_y = false;
    bool8 near_z = false;

    // NOTE: Scale does not support global mode.
    bool8 is_in_local_mode = mode == GizmoMode::Local || type == Rendering::RenderGizmoType::Scale;
    Vector3 right = is_in_local_mode ? TransformUtilities::GetRight(derived_transform) : Vector3(1.0f, 0.0f, 0.0f);
    Vector3 up = is_in_local_mode ? TransformUtilities::GetUp(derived_transform) : Vector3(0.0f, 1.0f, 0.0f);
    Vector3 forward = is_in_local_mode ? -TransformUtilities::GetForward(derived_transform) : Vector3(0.0f, 0.0f, 1.0f);
    
    Vector3 position = derived_transform->position;
    GizmoManipulation result = { };
    if (type == Rendering::RenderGizmoType::Translate) {
      Ray x_axis_ray = Ray(position, right);
      Ray y_axis_ray = Ray(position, up);
      Ray z_axis_ray = Ray(position, forward);
      Vector3 x_handle_position = position + 0.5f * right;
      Vector3 y_handle_position = position + 0.5f * up;
      Vector3 z_handle_position = position + 0.5f * forward;

      float32 distance_to_x = Ray::DistanceBetweenRays(x_axis_ray, ray);
      Vector3 x_axis_point = x_axis_ray.GetClosestPointToRay(ray);
      float32 distance_to_y = Ray::DistanceBetweenRays(y_axis_ray, ray);
      Vector3 y_axis_point = y_axis_ray.GetClosestPointToRay(ray);
      float32 distance_to_z = Ray::DistanceBetweenRays(z_axis_ray, ray);
      Vector3 z_axis_point = z_axis_ray.GetClosestPointToRay(ray);

      near_x = x_handle_position.Distance(x_axis_point) < 0.5f && std::abs(distance_to_x) < 0.1f;
      near_y = y_handle_position.Distance(y_axis_point) < 0.5f && std::abs(distance_to_y) < 0.1f;
      near_z = z_handle_position.Distance(z_axis_point) < 0.5f && std::abs(distance_to_z) < 0.1f;

      if (near_x) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = derived_transform->position - x_axis_point;
          should_transform_x = true;
        }
      } else if (near_y) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = derived_transform->position - y_axis_point;
          should_transform_y = true;
        }
      } else if (near_z) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = derived_transform->position - z_axis_point;
          should_transform_z = true;
        }
      }

      if (Input::IsMouseButtonHold(MouseButtonCode::Left)) {
        if (should_transform_x) {
          local_transform->position = TransformUtilities::WorldToLocalPosition(entity_manager, entity, x_axis_point + offset);
        } else if (should_transform_y) {
          local_transform->position = TransformUtilities::WorldToLocalPosition(entity_manager, entity, y_axis_point + offset);
        } else if (should_transform_z) {
          local_transform->position = TransformUtilities::WorldToLocalPosition(entity_manager, entity, z_axis_point + offset);
        }
      }
    } else if (type == Rendering::RenderGizmoType::Rotate) {
      Circle x_circle = { position, right, 1.0f };
      Circle y_circle = { position, up, 1.0f };
      Circle z_circle = { position, forward, 1.0f };

      Vector3 x_circle_point = Vector3();
      float32 distance_to_x = x_circle.GetClosestPointToRay(ray, x_circle_point);
      Vector3 y_circle_point = Vector3();
      float32 distance_to_y = y_circle.GetClosestPointToRay(ray, y_circle_point);
      Vector3 z_circle_point = Vector3();
      float32 distance_to_z = z_circle.GetClosestPointToRay(ray, z_circle_point);

      near_x = distance_to_x < 0.1f;
      near_y = distance_to_y < 0.1f;
      near_z = distance_to_z < 0.1f;

      if (near_x) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = x_circle_point;
          should_transform_x = true;
        }
      } else if (near_y) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = y_circle_point;
          should_transform_y = true;
        }
      } else if (near_z) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = z_circle_point;
          should_transform_z = true;
        }
      }

      if (Input::IsMouseButtonHold(MouseButtonCode::Left)) {
        near_x = false;
        near_y = false;
        near_z = false;
        
        if (should_transform_x) {
          Vector3 current = x_circle_point;
          float32 degree = x_circle.GetAngleBetweenPointsOnCircle(current, offset);

          if (mode == GizmoMode::Local) {
            right = local_transform->rotation * Vector3(1.0f, 0.0f, 0.0f);
          }
          Quaternion new_rotation = Quaternion::FromAxisAngle(right, degree) * local_transform->rotation;
          local_transform->rotation = new_rotation;

          offset = current;

          near_x = true;
        } else if (should_transform_y) {
          Vector3 current = y_circle_point;
          float32 degree = y_circle.GetAngleBetweenPointsOnCircle(current, offset);

          if (mode == GizmoMode::Local) {
            up = local_transform->rotation * Vector3(0.0f, 1.0f, 0.0f);
          }
          Quaternion new_rotation = Quaternion::FromAxisAngle(up, degree) * local_transform->rotation;
          local_transform->rotation = new_rotation;

          offset = current;

          near_y = true;
        } else if (should_transform_z) {
          Vector3 current = z_circle_point;
          float32 degree = z_circle.GetAngleBetweenPointsOnCircle(current, offset);

          if (mode == GizmoMode::Local) {
            forward = local_transform->rotation * Vector3(0.0f, 0.0f, 1.0f);
          }
          Quaternion new_rotation = Quaternion::FromAxisAngle(forward, degree) * local_transform->rotation;
          local_transform->rotation = new_rotation;

          offset = current;

          near_z = true;
        }
      }
    } else if (type == Rendering::RenderGizmoType::Scale) {
      Ray x_axis_ray = Ray(position, right);
      Ray y_axis_ray = Ray(position, up);
      Ray z_axis_ray = Ray(position, forward);
      Vector3 x_handle_position = position + 0.5f * right;
      Vector3 y_handle_position = position + 0.5f * up;
      Vector3 z_handle_position = position + 0.5f * forward;

      float32 distance_to_x = Ray::DistanceBetweenRays(x_axis_ray, ray);
      Vector3 x_axis_point = x_axis_ray.GetClosestPointToRay(ray);
      float32 distance_to_y = Ray::DistanceBetweenRays(y_axis_ray, ray);
      Vector3 y_axis_point = y_axis_ray.GetClosestPointToRay(ray);
      float32 distance_to_z = Ray::DistanceBetweenRays(z_axis_ray, ray);
      Vector3 z_axis_point = z_axis_ray.GetClosestPointToRay(ray);

      near_x = x_handle_position.Distance(x_axis_point) < 0.5f && std::abs(distance_to_x) < 0.1f;
      near_y = y_handle_position.Distance(y_axis_point) < 0.5f && std::abs(distance_to_y) < 0.1f;
      near_z = z_handle_position.Distance(z_axis_point) < 0.5f && std::abs(distance_to_z) < 0.1f;

      Vector3 x_scale_axis = right + Vector3(position.x, 0.0f, 0.0f);
      Vector3 y_scale_axis = up + Vector3(0.0f, position.y, 0.0f);
      Vector3 z_scale_axis = forward + Vector3(0.0f, 0.0f, position.z);
      
      if (near_x) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = local_transform->scale;
          offset.x -= x_axis_point.Dot(x_scale_axis);
          should_transform_x = true;
        }
      } else if (near_y) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = local_transform->scale;
          offset.y -= y_axis_point.Dot(y_scale_axis);
          should_transform_y = true;
        }
      } else if (near_z) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = local_transform->scale;
          offset.z -= z_axis_point.Dot(z_scale_axis);
          should_transform_z = true;
        }
      }

      if (Input::IsMouseButtonHold(MouseButtonCode::Left)) {
        if (should_transform_x) {
          float32 scale_x = x_axis_point.Dot(x_scale_axis);
          local_transform->scale.x = scale_x + offset.x;
        } else if (should_transform_y) {
          float32 scale_y = y_axis_point.Dot(y_scale_axis);
          local_transform->scale.y = scale_y + offset.y;
        } else if (should_transform_z) {
          float32 scale_z = z_axis_point.Dot(z_scale_axis);
          local_transform->scale.z = scale_z + offset.z;
        }
      }
    }

    if (Input::IsMouseButtonUp(MouseButtonCode::Left)) {
      should_transform_x = false;
      should_transform_y = false;
      should_transform_z = false;
    }
    
    result.in_transformation = should_transform_x || should_transform_y || should_transform_z; 
    if (near_x || should_transform_x) {
      result.highlight_axis = Rendering::RenderGizmoAxisHighlight::X;
    } else if (near_y || should_transform_y) {
      result.highlight_axis = Rendering::RenderGizmoAxisHighlight::Y;
    } else if (near_z || should_transform_z) {
      result.highlight_axis = Rendering::RenderGizmoAxisHighlight::Z;
    }
    
    return result;
  }
  
}
