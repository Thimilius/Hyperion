#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/core/math/quaternion.hpp"
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct LocalTransformComponent {
        Vector3 position = Vector3::Zero();
        Quaternion rotation = Quaternion::Identity();
        Vector3 scale = Vector3::One();
    };

    struct DerivedTransformComponent {
        Vector3 derived_position = Vector3::Zero();
        Quaternion derived_rotation = Quaternion::Identity();
        Vector3 derived_scale = Vector3::One();
    };

    struct LocalToWorldComponent {
        Matrix4x4 local_to_world = Matrix4x4::Identity();
    };

    struct HierarchyComponent {
        EntityId parent = Entity::EMPTY;

        uint64 child_count = 0;
        EntityId first_child = Entity::EMPTY;

        EntityId previous_sibling = Entity::EMPTY;
        EntityId next_sibling = Entity::EMPTY;
    };

}