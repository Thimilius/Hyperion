#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/core/math/quaternion.hpp"
#include "hyperion/ecs/entity/entity_types.hpp"
#include "hyperion/rendering/camera_projection_mode.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct TagComponent {
        String tag;
    };

    struct TransformComponent {
        Vector3 position = Vector3::Zero();
        Quaternion rotation = Quaternion::Identity();
        Vector3 scale = Vector3::One();
    };

    struct LocalToWorldComponent {
        Matrix4x4 local_to_world;
    };

    struct HierarchyComponent {
        uint64 child_count = 0;

        EntityId parent = Entity::EMPTY;

        EntityId first = Entity::EMPTY;
        EntityId prev = Entity::EMPTY;
        EntityId next = Entity::EMPTY;
    };

    struct CameraComponent {
        Rendering::CameraProjectionMode projection_mode = Rendering::CameraProjectionMode::Perspective;

        float32 near_plane = 0.1f;
        float32 far_plane = 1000.0f;

        float32 fov = 90.0f;
        float32 orthographic_size = 2.75f;

        Matrix4x4 view_matrix = Matrix4x4::Identity();
        Matrix4x4 projection_matrix = Matrix4x4::Identity();
    };

}