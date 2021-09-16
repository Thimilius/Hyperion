#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/ray.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/ecs/component/components/render_components.hpp"
#include "hyperion/ecs/component/components/transform_components.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class CameraUtilities final {
    public:
        static Vector3 ScreenToWorldPoint(CameraComponent *camera, Vector3 screen_point);
        static Vector2 WorldToScreenPoint(CameraComponent *camera, Vector3 world_point);
        static Ray ScreenPointToRay(CameraComponent *camera, LocalTransformComponent *local_transform, Vector2 screen_point);

        static void RecalculateMatricies(CameraComponent *camera, LocalTransformComponent *local_transform);
    private:
        CameraUtilities() = delete;
        ~CameraUtilities() = delete;
    };

}