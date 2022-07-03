//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/pipeline/render_pipeline_context.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/shader.hpp"
#include "hyperion/ecs/component/components/utilities/camera_utilities.hpp"
#include "hyperion/render/render_frame.hpp"
#include "hyperion/render/render_gizmos.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void RenderPipelineContext::Clear() {
    m_commands.Clear();
    m_async_request_results.Clear();
  }
  
  //--------------------------------------------------------------
  CullingResults RenderPipelineContext::Cull(CullingParameters parameters) {
    HYP_PROFILE_SCOPE("RenderFrame.Cull")

    CullingResults result;

    if (parameters.mask != LayerMask::Nothing) {
      const Array<RenderObjectContextMesh> &meshes = m_render_frame->GetObjectContext().GetMeshes();
      
      Array<Plane> frustum_planes = CameraUtilities::ExtractFrustumPlanes(parameters.matrix);
      uint32 index = 0;
      for (const RenderObjectContextMesh &object : meshes) {
        bool8 is_in_layer = (object.layer_mask & parameters.mask) == object.layer_mask && object.layer_mask != LayerMask::Nothing;
        if (is_in_layer) {
          bool8 is_inside_frustum = CameraUtilities::IsInsideFrustum(frustum_planes, object.bounds);
          if (is_inside_frustum) {
            result.visible_objects.Add(index);  
          }
        }

        index++;
      }
    }
    
    return result;
  }

  //--------------------------------------------------------------
  void RenderPipelineContext::SetCamera(uint64 camera_index) {
    RenderPipelineCommandSetCamera set_camera = { };
    set_camera.camera_index = camera_index;

    RenderPipelineCommand &command = CreateCommand(RenderPipelineCommandType::SetCamera);
    command.data = set_camera;
  }

  //--------------------------------------------------------------
  void RenderPipelineContext::ExecuteCommandBuffer(const RenderCommandBuffer &command_buffer) {
    RenderPipelineCommandExecuteCommandBuffer execute_command_buffer = { };
    execute_command_buffer.command_buffer = command_buffer;

    RenderPipelineCommand &command = CreateCommand(RenderPipelineCommandType::ExecuteCommandBuffer);
    command.data = execute_command_buffer;
  }

  //--------------------------------------------------------------
  void RenderPipelineContext::DrawMeshes(CullingResults &culling_results, DrawingParameters drawing_parameters) {
    RenderPipelineCommandDrawMeshes draw_meshes = { };
    draw_meshes.culling_results = culling_results;
    draw_meshes.drawing_parameters = drawing_parameters;
    draw_meshes.sorted_objects = culling_results.visible_objects;

    // We do the sorting on the Main Thread.
    {
      HYP_PROFILE_SCOPE("RenderFrame.SortObjects")

      const Array<RenderObjectContextMesh> &mesh_objects = m_render_frame->GetObjectContext().GetMeshes();
      Vector3 camera_position = drawing_parameters.sorting_settings.camera_position;

      switch (drawing_parameters.sorting_settings.criteria) {
        case SortingCriteria::None: break;
        case SortingCriteria::Opaque: {
          std::sort(draw_meshes.sorted_objects.begin(), draw_meshes.sorted_objects.end(), [mesh_objects, camera_position](uint32 a, uint32 b) {
            const RenderObjectContextMesh &mesh_a = mesh_objects[a];
            const RenderObjectContextMesh &mesh_b = mesh_objects[b];

            float32 distance_sqr_a = (camera_position - mesh_a.position).SqrMagnitude();
            float32 distance_sqr_b = (camera_position - mesh_b.position).SqrMagnitude();

            return distance_sqr_a < distance_sqr_b;
          });
          break;
        }
        case SortingCriteria::Transparent: {
          std::sort(draw_meshes.sorted_objects.begin(), draw_meshes.sorted_objects.end(), [mesh_objects, camera_position](uint32 a, uint32 b) {
            const RenderObjectContextMesh &mesh_a = mesh_objects[a];
            const RenderObjectContextMesh &mesh_b = mesh_objects[b];

            float32 distance_sqr_a = (camera_position - mesh_a.position).SqrMagnitude();
            float32 distance_sqr_b = (camera_position - mesh_b.position).SqrMagnitude();

            return distance_sqr_a > distance_sqr_b;
          });
          break;
        }
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
          break;
      }
    }

    RenderPipelineCommand &command = CreateCommand(RenderPipelineCommandType::DrawMeshes);
    command.data = draw_meshes;
  }

  //--------------------------------------------------------------
  void RenderPipelineContext::DrawShadows(ShadowParameters shadow_parameters) {
    RenderPipelineCommandDrawShadows draw_shadows = { };
    draw_shadows.shadow_parameters = shadow_parameters;

    RenderPipelineCommand &command = CreateCommand(RenderPipelineCommandType::DrawShadows);
    command.data = draw_shadows;
  }

  //--------------------------------------------------------------
  void RenderPipelineContext::DrawUI() {
    RenderPipelineCommandDrawUI draw_ui = { };

    RenderPipelineCommand &command = CreateCommand(RenderPipelineCommandType::DrawUI);
    command.data = draw_ui;
  }

  //--------------------------------------------------------------
  void RenderPipelineContext::DrawObjectIds(RenderTargetId render_target_id) {
    RenderPipelineCommandDrawObjectIds draw_object_ids = { };
    draw_object_ids.render_target_id = render_target_id;

    RenderPipelineCommand &command = CreateCommand(RenderPipelineCommandType::DrawObjectIds);
    command.data = draw_object_ids;
  }

  //--------------------------------------------------------------
  void RenderPipelineContext::DrawGizmos() {
    RenderPipelineCommandDrawGizmos draw_gizmos = { };
    draw_gizmos.shader_handle = AssetManager::GetShaderPrimitive(ShaderPrimitive::Gizmo)->GetMetadata().handle;
    
    draw_gizmos.grid.should_draw = RenderGizmos::GetShouldDrawGrid();
    draw_gizmos.grid.local_to_world = Matrix4x4::Identity();
    Mesh *grid_mesh = RenderGizmos::GetGridMesh();
    if (grid_mesh) {
      draw_gizmos.grid.mesh_handle = grid_mesh->GetMetadata().handle;
    } else {
      draw_gizmos.grid.mesh_handle = AssetHandle();
    }
    
    draw_gizmos.highlight.should_draw = RenderGizmos::GetShouldDrawHighlight();
    draw_gizmos.highlight.local_to_world = RenderGizmos::GetHighlightTransformation();
    Mesh *highlight_mesh = RenderGizmos::GetHighlightMesh();
    if (highlight_mesh) {
      draw_gizmos.highlight.mesh_handle = highlight_mesh->GetMetadata().handle;
    } else {
      draw_gizmos.highlight.mesh_handle = AssetHandle();
    }
    
    draw_gizmos.transformation_gizmo.should_draw = RenderGizmos::GetShouldDrawTransformationGizmo();
    draw_gizmos.transformation_gizmo.local_to_world = RenderGizmos::GetTransformationGizmoTransformation();
    Mesh *transformation_gizmo_mesh = RenderGizmos::GetTransformationGizmoMesh();
    if (transformation_gizmo_mesh) {
      draw_gizmos.transformation_gizmo.mesh_handle = transformation_gizmo_mesh->GetMetadata().handle;
    } else {
      draw_gizmos.transformation_gizmo.mesh_handle = AssetHandle();
    }
    
    draw_gizmos.should_draw_mesh_bounds = RenderGizmos::GetShouldDrawMeshBounds();

    RenderPipelineCommand &command = CreateCommand(RenderPipelineCommandType::DrawGizmos);
    command.data = draw_gizmos;
  }

  //--------------------------------------------------------------
  void RenderPipelineContext::DrawEditorUI() {
    RenderPipelineCommandDrawEditorUI draw_editor_ui = { };

    RenderPipelineCommand &command = CreateCommand(RenderPipelineCommandType::DrawEditorUI);
    command.data = draw_editor_ui;
  }

  //--------------------------------------------------------------
  AsyncRequestResult &RenderPipelineContext::AddAsyncRequestResult() {
    m_async_request_results.Resize(m_async_request_results.GetLength() + 1);
    return m_async_request_results.GetLast();
  }

  //--------------------------------------------------------------
  RenderPipelineCommand &RenderPipelineContext::CreateCommand(RenderPipelineCommandType type) {
    m_commands.Resize(m_commands.GetLength() + 1);
    RenderPipelineCommand &command = m_commands.GetLast();
    command.type = type;
    return command;
  }
  
}
