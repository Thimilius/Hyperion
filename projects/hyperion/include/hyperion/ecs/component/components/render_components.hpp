#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/rendering/render_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct CameraComponent {
        Rendering::CameraProjectionMode projection_mode = Rendering::CameraProjectionMode::Perspective;

        Rendering::CameraClearMode clear_mode = Rendering::CameraClearMode::Color;
        Color background_color = Color::Black();

        Rendering::RenderLayerMask visibility_mask = Rendering::RenderLayerMask::Everything;

        float32 near_plane = 0.1f;
        float32 far_plane = 1000.0f;

        float32 fov = 90.0f;
        float32 orthographic_size = 2.75f;

        Rendering::CameraViewportClipping viewport_clipping = { 0.0f, 0.0f, 1.0f, 1.0f };

        Matrix4x4 view_matrix = Matrix4x4::Identity();
        Matrix4x4 projection_matrix = Matrix4x4::Identity();
        Matrix4x4 view_projection_matrix = Matrix4x4::Identity();

        Matrix4x4 inverse_view_matrix = Matrix4x4::Identity();
        Matrix4x4 inverse_projection_matrix = Matrix4x4::Identity();
        Matrix4x4 inverse_view_projection_matrix = Matrix4x4::Identity();
    };

    struct SpriteComponent {
        Color color = Color::White();
        Texture2D *texture = nullptr;
    };

    struct RenderMeshComponent {
        Mesh *mesh = nullptr;
        uint32 sub_mesh_index = 0;

        Material *material = nullptr;

        Rendering::RenderLayerMask layer_mask = Rendering::RenderLayerMask::Default;
    };

}