//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/render/types/render_types_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  using RenderFrameContextObjectId = uint64;

  struct RenderFrameContextObject {
    RenderFrameContextObjectId id = 0;
    Matrix4x4 local_to_world;
    Vector3 position;
  };

  struct RenderFrameContextObjectMesh : public RenderFrameContextObject {
    AssetHandle mesh_handle;
    uint32 sub_mesh_index = 0;

    AssetHandle shader_handle;
    AssetHandle material_handle;

    LayerMask layer_mask = LayerMask::Default;

    BoundingBox bounds;
  };

  struct RenderFrameContextObjectSprite : public RenderFrameContextObject {
    Color color;
  };

  struct RenderFrameContextObjectUI : public RenderFrameContextObject {
    AssetHandle mesh_handle;

    AssetHandle shader_handle;
    AssetHandle material_handle;

    Color color;
    ShaderPropertyStorage::Texture texture;

    RectInt scissor;
    bool8 enable_blending = false;
  };

}
