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

    static void RecalculateMatricies(CameraComponent *camera, DerivedTransformComponent *derived_transform);
    static CameraViewport CalculateViewportFromClipping(CameraViewportClipping viewport_clipping);

    static Array<Plane> ExtractFrustumPlanes(Matrix4x4 view_projection);
    static bool IsInsideFrustum(const Array<Plane> &frustum_planes, BoundingBox bounds);
  private:
    CameraUtilities() = delete;
    ~CameraUtilities() = delete;
  };

}