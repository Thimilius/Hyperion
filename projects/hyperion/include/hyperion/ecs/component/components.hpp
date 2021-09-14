#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/core/math/quaternion.hpp"
#include "hyperion/ecs/entity/entity_types.hpp"
#include "hyperion/rendering/render_types.hpp"

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
        Matrix4x4 local_to_world = Matrix4x4::Identity();
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

        Rendering::CameraClearMode clear_mode = Rendering::CameraClearMode::Color;
        Color background_color = Color::Black();

        float32 near_plane = 0.1f;
        float32 far_plane = 1000.0f;

        float32 fov = 90.0f;
        float32 orthographic_size = 2.75f;

        Rendering::CameraViewportClipping viewport_clipping = { 0.0f, 0.0f, 1.0f, 1.0f };
    };

    struct SpriteComponent {
        Color color = Color::White();
        Texture2D *texture = nullptr;
    };

    struct RenderMeshComponent {
        Mesh *mesh = nullptr;
        uint32 sub_mesh_index = 0;
    };

}