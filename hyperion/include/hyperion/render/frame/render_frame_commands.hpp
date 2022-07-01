//------------------------ Header Guard ------------------------
#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <variant>

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/render/command/render_command_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  enum class RenderFrameCommandType {
    None,
    
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

  struct ShadowSettings {
    RenderTargetId shadow_map_render_target_id;
    Matrix4x4 light_space_matrix;
  };
  
  struct DrawingParameters {
    LayerMask filter_mask = LayerMask::Everything;
    PerObjectData per_object_data = PerObjectData::Nothing;
    ShaderRenderOrder render_order = ShaderRenderOrder::Opaque;
    SortingSettings sorting_settings;
    ShadowSettings shadow_settings;
  };

  struct ShadowParameters {
    uint32 light_index = 0;
    uint32 shadow_map_size = 0;
    Matrix4x4 light_space_matrix;
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
    struct GizmoInstance {
      bool8 should_draw = false;
      Matrix4x4 local_to_world;
      AssetHandle mesh_handle;
    };
    
    AssetHandle shader_handle;

    GizmoInstance grid;
    GizmoInstance highlight;
    GizmoInstance transformation_gizmo;

    bool8 should_draw_mesh_bounds;
  };

  struct RenderFrameCommandDrawEditorUI { };

  struct RenderFrameCommand {
    RenderFrameCommandType type = RenderFrameCommandType::None;

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
