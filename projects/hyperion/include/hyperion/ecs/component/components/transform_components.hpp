#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/core/math/quaternion.hpp"
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct TransformComponent {
        Vector3 position = Vector3::Zero();
        Quaternion rotation = Quaternion::Identity();
        Vector3 scale = Vector3::One();
    };

    struct LocalToWorldComponent {
        Matrix4x4 local_to_world = Matrix4x4::Identity();
    };

    struct HierarchyComponent {
        uint64 child_count = 0;

        EntityId parent = Entity::EMPTY;

        EntityId first = Entity::EMPTY;
        EntityId prev = Entity::EMPTY;
        EntityId next = Entity::EMPTY;
    };

}