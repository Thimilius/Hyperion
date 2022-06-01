//------------------------ Header Guard ------------------------
#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <variant>

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/render/render_gizmos.hpp"
#include "hyperion/render/command/render_command_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  enum class RenderFrameCommandType {
    SetCamera,

    ExecuteCommandBuffer,

    DrawMeshes,
    DrawShadows,
    DrawUI,
    DrawGizmos,
    DrawObjectIds,

    DrawEditorUI,
  };

  struct RenderFrameCommandSetCamera {
    uint64 camera_index;
  };

  struct RenderFrameCommandExecuteCommandBuffer {
    RenderCommandBuffer command_buffer;
  };

  struct CullingResults {
    Array<uint32> visible_objects;
  };

  enum PerObjectData {
    Nothing,

    LightIndices
  };

  HYP_CREATE_ENUM_FLAG_OPERATORS(PerObjectData);

  enum class SortingCriteria {
    None,

    Opaque,
    Transparent
  };

  struct SortingSettings {
    SortingCriteria criteria = SortingCriteria::Opaque;
    Vector3 camera_position;
  };

  struct DrawingParameters {
    LayerMask filter_mask = LayerMask::Everything;
    PerObjectData per_object_data = PerObjectData::Nothing;
    ShaderRenderOrder render_order = ShaderRenderOrder::Opaque;
    SortingSettings sorting_settings;
  };

  struct ShadowParameters {
    uint32 light_index = 0;
  };

  struct RenderFrameCommandDrawMeshes {
    CullingResults culling_results;
    DrawingParameters drawing_parameters;
    Array<uint32> sorted_objects;
  };

  struct RenderFrameCommandDrawShadows {
    ShadowParameters shadow_parameters;
  };

  struct RenderFrameCommandDrawUI { };

  struct RenderFrameCommandDrawObjectIds {
    RenderTargetId render_target_id;
  };

  struct RenderFrameCommandDrawGizmos {
    AssetId shader_id;

    struct Grid {
      bool8 should_draw;
      Matrix4x4 local_to_world;
      RenderGizmoGridType type;
      AssetId mesh_id;
    } grid;

    struct TransformationGizmo {
      bool8 should_draw;
      Matrix4x4 local_to_world;
      AssetId mesh_id;
    } transformation_gizmo;

    bool8 should_draw_all_bounds;
  };

  struct RenderFrameCommandDrawEditorUI { };

  struct RenderFrameCommand {
    RenderFrameCommandType type;

    std::variant<
      RenderFrameCommandSetCamera,
      RenderFrameCommandExecuteCommandBuffer,
      RenderFrameCommandDrawMeshes,
      RenderFrameCommandDrawShadows,
      RenderFrameCommandDrawUI,
      RenderFrameCommandDrawObjectIds,
      RenderFrameCommandDrawGizmos,
      RenderFrameCommandDrawEditorUI
    > data;
  };

}
