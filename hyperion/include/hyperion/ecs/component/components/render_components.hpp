//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/render/types/render_types_camera.hpp"
#include "hyperion/render/types/render_types_general.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  struct CameraComponent {
    CameraProjectionMode projection_mode = CameraProjectionMode::Perspective;

    CameraClearMode clear_mode = CameraClearMode::Color;
    Color background_color = Color::Black();

    LayerMask culling_mask = LayerMask::Everything;

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

  struct SpriteComponent {
    Color color = Color::White();
    AssetHandle texture;
  };

  struct MeshComponent {
    AssetHandle mesh;
    uint32 sub_mesh_index = 0;

    AssetHandle material;

    LayerMask layer_mask = LayerMask::Default;
  };

  struct MeshBoundsComponent {
    BoundingBox bounds;
  };

  struct LightComponent {
    HYP_REFLECT()
  public:
    virtual ~LightComponent() = default;
    
    float32 intensity = 1.0f;
    Color color = Color::White();
  protected:
    LightComponent() = default;
  };

  struct DirectionalLightComponent : public LightComponent {
    HYP_REFLECT(LightComponent)
  };

  struct PointLightComponent : public LightComponent {
    HYP_REFLECT(LightComponent)
  public:
    float32 range = 10.0f;
  };

  struct SpotLightComponent : public LightComponent {
    HYP_REFLECT(LightComponent)
  public:
    float32 inner_spot_radius = 1.0f;
    float32 outer_spot_radius = 1.0f;
  };

}
