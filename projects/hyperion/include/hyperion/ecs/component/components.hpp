#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/core/math/quaternion.hpp"
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct TagComponent {
        String tag;
    };

    struct TransformComponent {
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;
    };

    struct HierarchyComponent {
        uint64 child_count;

        EntityId parent = Entity::EMPTY;

        EntityId first = Entity::EMPTY;
        EntityId prev = Entity::EMPTY;
        EntityId next = Entity::EMPTY;
    };

}