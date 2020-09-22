#pragma once

#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/ray.hpp"

namespace Hyperion {
    class Collider;
    class BoxCollider;
    class SphereCollider;
}

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

        virtual void DebugDraw() { }
    private:
        virtual void AddBoxCollider(BoxCollider *box_collider) { }
        virtual void AddSphereCollider(SphereCollider *sphere_collider) { }

        virtual void RemoveCollider(Collider *collider) { }

        virtual void UpdateBoxCollider(BoxCollider *box_collider) { }
        virtual void UpdateSphereCollider(SphereCollider *sphere_collider) { }
        virtual void UpdateBoxColliderTransform(BoxCollider *box_collider) { }
        virtual void UpdateSphereColliderTransform(SphereCollider *sphere_collider) { }
        virtual void UpdateColliderActivation(Collider *collider) { }

        virtual BoundingBox GetBounds(Collider *collider) { return BoundingBox(); }
    private:
        friend class Hyperion::Collider;
        friend class Hyperion::BoxCollider;
        friend class Hyperion::SphereCollider;
    };

}