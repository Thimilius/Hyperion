#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/ecs/component/component.hpp"
#include "hyperion/render/types/render_types_camera.hpp"
#include "hyperion/render/types/render_types_general.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct CameraComponent : public IComponent {
        CameraProjectionMode projection_mode = CameraProjectionMode::Perspective;

        CameraClearMode clear_mode = CameraClearMode::Color;
        Color background_color = Color::Black();

        LayerMask visibility_mask = LayerMask::Everything;

        float32 near_plane = 0.1f;
        float32 far_plane = 1000.0f;

        float32 fov = 90.0f;
        float32 orthographic_size = 2.75f;

        CameraViewportClipping viewport_clipping = { 0.0f, 0.0f, 1.0f, 1.0f };

        Matrix4x4 view_matrix = Matrix4x4::Identity();
        Matrix4x4 projection_matrix = Matrix4x4::Identity();
        Matrix4x4 view_projection_matrix = Matrix4x4::Identity();

        Matrix4x4 inverse_view_matrix = Matrix4x4::Identity();
        Matrix4x4 inverse_projection_matrix = Matrix4x4::Identity();
        Matrix4x4 inverse_view_projection_matrix = Matrix4x4::Identity();
    };

    struct SpriteComponent : public IComponent {
        Color color = Color::White();
        Texture2D *texture = nullptr;
    };

    struct MeshComponent : public IComponent {
        Mesh *mesh = nullptr;
        uint32 sub_mesh_index = 0;

        Material *material = nullptr;

        LayerMask layer_mask = LayerMask::Default;
    };

    struct LocalMeshBoundsComponent : public IComponent {
        BoundingBox bounds;
    };

    struct WorldMeshBoundsComponent : public IComponent {
        BoundingBox bounds;
    };

    struct LightComponent : public IComponent {
        float32 intensity = 1.0f;
        Color color = Color::White();
    };

    struct DirectionalLightComponent : public LightComponent {
          
    };

    struct PointLightComponent : public LightComponent {
        float32 range = 10.0f;
    };

    struct SpotLightComponent : public LightComponent {
        float32 inner_spot_radius = 1.0f;
        float32 outer_spot_radius = 1.0f;
    };

}