//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  using RenderFrameContextObjectId = uint32;

  struct RenderFrameContextObject {
    RenderFrameContextObjectId id;
    Matrix4x4 local_to_world;
    Vector3 position;
  };

  struct RenderFrameContextObjectMesh : public RenderFrameContextObject {
    AssetId mesh_id;
    uint32 sub_mesh_index;

    AssetId shader_id;
    AssetId material_id;

    LayerMask layer_mask;

    BoundingBox bounds;
  };

  struct RenderFrameContextObjectSprite : public RenderFrameContextObject {
    Color color;
  };

  struct RenderFrameContextObjectUI : public RenderFrameContextObject {
    AssetId mesh_id;

    AssetId shader_id;
    AssetId material_id;

    Color color;
    ShaderPropertyStorage::Texture texture;

    bool8 enable_blending;
  };

}