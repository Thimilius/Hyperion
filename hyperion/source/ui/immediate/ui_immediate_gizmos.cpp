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
  bool8 UIImmediateGizmos::Manipulate(
    DerivedTransformComponent *derived_transform,
    LocalTransformComponent *local_transform,
    DerivedTransformComponent *camera_transform,
    Ray ray) {
    auto get_closest_t_for_first_ray = [](Ray l1, Ray l2) -> float {
      const Vector3 dp = l2.origin - l1.origin;
      const float v12 = l1.direction.Dot(l1.direction);
      const float v22 = l2.direction.Dot(l2.direction);
      const float v1v2 = l1.direction.Dot(l2.direction);

      const float det = v1v2 * v1v2 - v12 * v22;

      if (std::abs(det) > FLT_MIN)
      {
        const float inv_det = 1.f / det;

        const float dpv1 = dp.Dot(l1.direction);
        const float dpv2 = dp.Dot(l2.direction);

        auto l1t = inv_det * (v22 * dpv1 - v1v2 * dpv2);
        auto l2t = inv_det * (v1v2 * dpv1 - v12 * dpv2);
        
        return -l1t;
      }
      else
      {
        const Vector3 a = Vector3::Cross(dp, l1.direction);
        return std::sqrt(a.Dot(a) / v12);
      }
    };

    auto distance_between_rays = [](Ray a, Ray b) {
      Vector3 cross = Vector3::Cross(a.direction, b.direction);
      float32 mag = cross.Magnitude();
      if (mag == 0.0f) {
        return 0.0f;
      }

      Vector3 p = b.origin - a.origin;
      float32 d = cross.Dot(p);

      return d / mag;
    };
    
    
    static Vector3 offset = Vector3();
    static bool8 should_move_x = false;
    static bool8 should_move_y = false;
    static bool8 should_move_z = false;
    
    Vector3 position = derived_transform->position;
    Ray x_axis_ray = Ray(Vector3(0.0f, position.y, position.z), Vector3(1, 0, 0));
    Ray y_axis_ray = Ray(Vector3(position.x, 0.0f, position.z), Vector3(0, 1, 0));
    Ray z_axis_ray = Ray(Vector3(position.x, position.y, 0.0f), Vector3(0, 0, 1));
    Vector3 x_handle_position = Vector3(position.x + 0.5f, position.y, position.z);
    Vector3 y_handle_position = Vector3(position.x, position.y + 0.5f, position.z);
    Vector3 z_handle_position = Vector3(position.x, position.y, position.z + 0.5f);

    float32 distance_to_x = distance_between_rays(x_axis_ray, ray);
    float32 x_t = get_closest_t_for_first_ray(x_axis_ray, ray);
    Vector3 x_axis_point = x_axis_ray.GetPoint(x_t);
    float32 distance_to_y = distance_between_rays(y_axis_ray, ray);
    float32 y_t = get_closest_t_for_first_ray(y_axis_ray, ray);
    Vector3 y_axis_point = y_axis_ray.GetPoint(y_t);
    float32 distance_to_z = distance_between_rays(z_axis_ray, ray);
    float32 z_t = get_closest_t_for_first_ray(z_axis_ray, ray);
    Vector3 z_axis_point = z_axis_ray.GetPoint(z_t);

    bool8 near_x = x_handle_position.Distance(x_axis_point) < 0.5f && std::abs(distance_to_x) < 0.1f;
    bool8 near_y = y_handle_position.Distance(y_axis_point) < 0.5f && std::abs(distance_to_y) < 0.1f;
    bool8 near_z = z_handle_position.Distance(z_axis_point) < 0.5f && std::abs(distance_to_z) < 0.1f;

    Plane xy_plane = Plane(Vector3(0.0f, 0.0f, 1.0f), -position.z);
    Plane xz_plane = Plane(Vector3(0.0f, 1.0f, 0.0f), -position.y); 
    Plane yz_plane = Plane(Vector3(1.0f, 0.0f, 0.0f), -position.x);

    Vector3 camera_direction = TransformUtilities::GetForward(camera_transform);
    float32 xy_plane_dot = Math::Abs(xy_plane.normal.Dot(camera_direction));
    float32 xz_plane_dot = Math::Abs(xz_plane.normal.Dot(camera_direction));
    float32 yz_plane_dot = Math::Abs(yz_plane.normal.Dot(camera_direction));
    
    if (near_x) {
      if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
        offset = local_transform->position - x_axis_point;
        should_move_x = true;
      }
    }
    if (near_y) {
      if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
        offset = local_transform->position - y_axis_point;
        should_move_y = true;
      }
    }
    if (near_z) {
      if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
        offset = local_transform->position - z_axis_point;
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
        
        local_transform->position = x_axis_point + offset;
      }
      if (should_move_y) {
        if (xy_plane_dot > yz_plane_dot) {
          plane = xy_plane;
        } else {
          plane = yz_plane;
        }
        plane.Intersects(ray, plane_distance);
        y_axis_point.y = ray.GetPoint(plane_distance).y;
        
        local_transform->position = y_axis_point + offset;
      }
      if (should_move_z) {
        if (xz_plane_dot > yz_plane_dot) {
          plane = xz_plane;
        } else {
          plane = yz_plane;
        }
        plane.Intersects(ray, plane_distance);
        z_axis_point.z = ray.GetPoint(plane_distance).z;
        
        local_transform->position = z_axis_point + offset;
      }
    }

    if (Input::IsMouseButtonUp(MouseButtonCode::Left)) {
      should_move_x = false;
      should_move_y = false;
      should_move_z = false;
    }

    return should_move_x || should_move_y || should_move_z;
  }
  
}
