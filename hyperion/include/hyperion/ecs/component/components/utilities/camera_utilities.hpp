//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/ray.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/core/math/plane.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/ecs/component/components/render_components.hpp"
#include "hyperion/ecs/component/components/transform_components.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class CameraUtilities final {
  public:
    static Vector3 ScreenToWorldPoint(CameraComponent *camera, Vector3 screen_point);
    static Vector2 WorldToScreenPoint(CameraComponent *camera, Vector3 world_point);
    static Ray ScreenPointToRay(CameraComponent *camera, DerivedTransformComponent *derived_transform, Vector2 screen_point);
    static Ray ScreenPointToRay(CameraComponent *camera, DerivedTransformComponent *derived_transform, Vector2 screen_point, Vector2 display_size);

    static void RecalculateMatrices(CameraComponent *camera, DerivedTransformComponent *derived_transform);
    static void RecalculateMatrices(CameraComponent *camera, DerivedTransformComponent *derived_transform, CameraViewport viewport);
    static void RecalculateMatrices(CameraComponent *camera, Vector3 position, Vector3 up, Vector3 forward, CameraViewport viewport);
    static CameraViewport CalculateViewportFromClipping(CameraViewportClipping viewport_clipping);
    static CameraViewport CalculateViewportFromClipping(CameraViewportClipping viewport_clipping, Vector2 display_size);

    static Array<Plane> ExtractFrustumPlanes(Matrix4x4 view_projection);
    static bool IsInsideFrustum(const Array<Plane> &frustum_planes, BoundingBox bounds);
  private:
    CameraUtilities() = delete;
    ~CameraUtilities() = delete;
  };

}
