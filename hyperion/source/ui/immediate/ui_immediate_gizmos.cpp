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
    EntityManager *entity_manager,
    EntityId entity,
    DerivedTransformComponent *derived_transform,
    LocalTransformComponent *local_transform,
    DerivedTransformComponent *camera_transform,
    Ray ray) {
    static Vector3 offset = Vector3();
    static bool8 should_move_x = false;
    static bool8 should_move_y = false;
    static bool8 should_move_z = false;

    bool8 near_x = false;
    bool8 near_y = false;
    bool8 near_z = false;
    
    Vector3 position = derived_transform->position;
    GizmoManipulation result = { };
    if (type == Rendering::RenderGizmoType::Translate) {
      Ray x_axis_ray = Ray(Vector3(0.0f, position.y, position.z), Vector3(1.0f, 0.0f, 0.0f));
      Ray y_axis_ray = Ray(Vector3(position.x, 0.0f, position.z), Vector3(0.0f, 1.0f, 0.0f));
      Ray z_axis_ray = Ray(Vector3(position.x, position.y, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
      Vector3 x_handle_position = Vector3(position.x + 0.5f, position.y, position.z);
      Vector3 y_handle_position = Vector3(position.x, position.y + 0.5f, position.z);
      Vector3 z_handle_position = Vector3(position.x, position.y, position.z + 0.5f);

      float32 distance_to_x = Ray::DistanceBetweenRays(x_axis_ray, ray);
      Vector3 x_axis_point = x_axis_ray.GetClosestPointToRay(ray);
      float32 distance_to_y = Ray::DistanceBetweenRays(y_axis_ray, ray);
      Vector3 y_axis_point = y_axis_ray.GetClosestPointToRay(ray);
      float32 distance_to_z = Ray::DistanceBetweenRays(z_axis_ray, ray);
      Vector3 z_axis_point = z_axis_ray.GetClosestPointToRay(ray);

      near_x = x_handle_position.Distance(x_axis_point) < 0.5f && std::abs(distance_to_x) < 0.1f;
      near_y = y_handle_position.Distance(y_axis_point) < 0.5f && std::abs(distance_to_y) < 0.1f;
      near_z = z_handle_position.Distance(z_axis_point) < 0.5f && std::abs(distance_to_z) < 0.1f;

      Plane xy_plane = Plane(Vector3(0.0f, 0.0f, 1.0f), -position.z);
      Plane xz_plane = Plane(Vector3(0.0f, 1.0f, 0.0f), -position.y); 
      Plane yz_plane = Plane(Vector3(1.0f, 0.0f, 0.0f), -position.x);

      Vector3 camera_direction = TransformUtilities::GetForward(camera_transform);
      float32 xy_plane_dot = Math::Abs(xy_plane.normal.Dot(camera_direction));
      float32 xz_plane_dot = Math::Abs(xz_plane.normal.Dot(camera_direction));
      float32 yz_plane_dot = Math::Abs(yz_plane.normal.Dot(camera_direction));
      
      if (near_x) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = derived_transform->position - x_axis_point;
          should_move_x = true;
        }
      } else if (near_y) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = derived_transform->position - y_axis_point;
          should_move_y = true;
        }
      } else if (near_z) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = derived_transform->position - z_axis_point;
          should_move_z = true;
        }
      }

      Plane plane;
      float32 plane_distance = 0.0f;
      if (Input::IsMouseButtonHold(MouseButtonCode::Left)) {
        if (should_move_x) {
          if (xy_plane_dot > xz_plane_dot) {
            plane = xy_plane;
          } else {
            plane = xz_plane;
          }
          plane.Intersects(ray, plane_distance);
          x_axis_point.x = ray.GetPoint(plane_distance).x;
          
          local_transform->position = TransformUtilities::WorldToLocalPosition(entity_manager, entity, x_axis_point + offset);
        } else if (should_move_y) {
          if (xy_plane_dot > yz_plane_dot) {
            plane = xy_plane;
          } else {
            plane = yz_plane;
          }
          plane.Intersects(ray, plane_distance);
          y_axis_point.y = ray.GetPoint(plane_distance).y;
          
          local_transform->position = TransformUtilities::WorldToLocalPosition(entity_manager, entity, y_axis_point + offset);
        } else if (should_move_z) {
          if (xz_plane_dot > yz_plane_dot) {
            plane = xz_plane;
          } else {
            plane = yz_plane;
          }
          plane.Intersects(ray, plane_distance);
          z_axis_point.z = ray.GetPoint(plane_distance).z;
          
          local_transform->position = TransformUtilities::WorldToLocalPosition(entity_manager, entity, z_axis_point + offset);
        }
      }
    } else if (type == Rendering::RenderGizmoType::Rotate) {
      Circle x_circle = { position, Vector3(1.0f, 0.0f, 0.0f), 1.0f };
      Circle y_circle = { position, Vector3(0.0f, 1.0f, 0.0f), 1.0f };
      Circle z_circle = { position, Vector3(0.0f, 0.0f, 1.0f), 1.0f };
      
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
          should_move_x = true;
        }
      } else if (near_y) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = y_circle_point;
          should_move_y = true;
        }
      } else if (near_z) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          offset = z_circle_point;
          should_move_z = true;
        }
      }

      if (Input::IsMouseButtonHold(MouseButtonCode::Left)) {
        near_x = false;
        near_y = false;
        near_z = false;
        
        if (should_move_x) {
          Vector3 current = x_circle_point;
          float32 degree = x_circle.GetAngleBetweenPoints(current, offset);
          
          Quaternion new_rotation = Quaternion::FromAxisAngle(Vector3(1.0f, 0.0f, 0.0f), degree) * local_transform->rotation;
          local_transform->rotation = new_rotation;

          offset = current;

          near_x = true;
        } else if (should_move_y) {
          Vector3 current = y_circle_point;
          float32 degree = y_circle.GetAngleBetweenPoints(current, offset);
          
          Quaternion new_rotation = Quaternion::FromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), degree) * local_transform->rotation;
          local_transform->rotation = new_rotation;

          offset = current;

          near_y = true;
        } else if (should_move_z) {
          Vector3 current = z_circle_point;
          float32 degree = z_circle.GetAngleBetweenPoints(current, offset);
          
          Quaternion new_rotation = Quaternion::FromAxisAngle(Vector3(0.0f, 0.0f, 1.0f), degree) * local_transform->rotation;
          local_transform->rotation = new_rotation;

          offset = current;

          near_z = true;
        }
      }
    }

    if (Input::IsMouseButtonUp(MouseButtonCode::Left)) {
      should_move_x = false;
      should_move_y = false;
      should_move_z = false;
    }
    
    result.in_transformation = should_move_x || should_move_y || should_move_z; 
    if (near_x || should_move_x) {
      result.highlight_axis = Rendering::RenderGizmoAxisHighlight::X;
    } else if (near_y || should_move_y) {
      result.highlight_axis = Rendering::RenderGizmoAxisHighlight::Y;
    } else if (near_z || should_move_z) {
      result.highlight_axis = Rendering::RenderGizmoAxisHighlight::Z;
    }
    
    return result;
  }
  
}
