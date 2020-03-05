#pragma once

#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/ray.hpp"
#include "hyperion/entity/components/physics/collider.hpp"

namespace Hyperion::Physics {

    struct RaycastResult {
        Vec3 point = Vec3::Zero();
        Vec3 normal = Vec3::Zero();

        Collider *collider = nullptr;
    };

    class PhysicsWorld {
    public:
        virtual ~PhysicsWorld() = default;

        virtual bool Raycast(Ray ray, RaycastResult &result, f32 distance) { return false; }
    };

}