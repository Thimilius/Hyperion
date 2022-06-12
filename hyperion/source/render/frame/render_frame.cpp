//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/frame/render_frame.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/shader.hpp"
#include "hyperion/ecs/component/components/utilities/camera_utilities.hpp"
#include "hyperion/render/render_gizmos.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void RenderFrame::Clear() {
    m_context.Clear();
    m_asset_context.Clear();
    m_commands.Clear();
    m_async_request_results.Clear();
  }

  //--------------------------------------------------------------
  CullingResults RenderFrame::Cull(CullingParameters parameters) {
    HYP_PROFILE_SCOPE("RenderFrame.Cull");

    Array<Plane> frustum_planes = CameraUtilities::ExtractFrustumPlanes(parameters.matrix);

    CullingResults result;

    uint32 index = 0;
    for (const RenderFrameContextObjectMesh &object : m_context.GetMeshObjects()) {
      bool8 is_inside_frustum = CameraUtilities::IsInsideFrustum(frustum_planes, object.bounds);
      bool8 is_in_layer = (object.layer_mask & parameters.mask) == object.layer_mask;

      if (is_inside_frustum && is_in_layer) {
        result.visible_objects.Add(index);
      }

      index++;
    }

    return result;
  }

  //--------------------------------------------------------------
  void RenderFrame::SetCamera(uint64 camera_index) {
    RenderFrameCommandSetCamera set_camera;
    set_camera.camera_index = camera_index;

    RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::SetCamera);
    command.data = set_camera;
  }

  //--------------------------------------------------------------
  void RenderFrame::ExecuteCommandBuffer(const RenderCommandBuffer &command_buffer) {
    RenderFrameCommandExecuteCommandBuffer execute_command_buffer;
    execute_command_buffer.command_buffer = command_buffer;

    RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::ExecuteCommandBuffer);
    command.data = execute_command_buffer;
  }

  //--------------------------------------------------------------
  void RenderFrame::DrawMeshes(CullingResults &culling_results, DrawingParameters drawing_parameters) {
    RenderFrameCommandDrawMeshes draw_meshes;
    draw_meshes.culling_results = culling_results;
    draw_meshes.drawing_parameters = drawing_parameters;
    draw_meshes.sorted_objects = culling_results.visible_objects;

    // We do the sorting on the Main Thread.
    {
      HYP_PROFILE_SCOPE("RenderFrame.SortObjects");

      const Array<RenderFrameContextObjectMesh> &mesh_objects = m_context.GetMeshObjects();
      Vector3 camera_position = drawing_parameters.sorting_settings.camera_position;

      switch (drawing_parameters.sorting_settings.criteria) {
        case SortingCriteria::None: break;
        case SortingCriteria::Opaque: {
          std::sort(draw_meshes.sorted_objects.begin(), draw_meshes.sorted_objects.end(), [mesh_objects, camera_position](uint32 a, uint32 b) {
            const RenderFrameContextObjectMesh &mesh_a = mesh_objects[a];
            const RenderFrameContextObjectMesh &mesh_b = mesh_objects[b];

            float32 distance_sqr_a = (camera_position - mesh_a.position).SqrMagnitude();
            float32 distance_sqr_b = (camera_position - mesh_b.position).SqrMagnitude();

            return distance_sqr_a < distance_sqr_b;
          });
          break;
        }
        case SortingCriteria::Transparent: {
          std::sort(draw_meshes.sorted_objects.begin(), draw_meshes.sorted_objects.end(), [mesh_objects, camera_position](uint32 a, uint32 b) {
            const RenderFrameContextObjectMesh &mesh_a = mesh_objects[a];
            const RenderFrameContextObjectMesh &mesh_b = mesh_objects[b];

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

    RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawMeshes);
    command.data = draw_meshes;
  }

  //--------------------------------------------------------------
  void RenderFrame::DrawShadows(ShadowParameters shadow_parameters) {
    RenderFrameCommandDrawShadows draw_shadows;
    draw_shadows.shadow_parameters = shadow_parameters;

    RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawShadows);
    command.data = draw_shadows;
  }

  //--------------------------------------------------------------
  void RenderFrame::DrawUI() {
    RenderFrameCommandDrawUI draw_ui;

    RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawUI);
    command.data = draw_ui;
  }

  //--------------------------------------------------------------
  void RenderFrame::DrawObjectIds(RenderTargetId render_target_id) {
    RenderFrameCommandDrawObjectIds draw_object_ids;
    draw_object_ids.render_target_id = render_target_id;

    RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawObjectIds);
    command.data = draw_object_ids;
  }

  //--------------------------------------------------------------
  void RenderFrame::DrawGizmos() {
    RenderFrameCommandDrawGizmos draw_gizmos;
    draw_gizmos.shader_handle = AssetManager::GetShaderPrimitive(ShaderPrimitive::Gizmo)->GetAssetInfo().handle;
    draw_gizmos.grid.should_draw = RenderGizmos::GetShouldDrawGrid();
    draw_gizmos.grid.local_to_world = Matrix4x4::Identity();
    draw_gizmos.grid.type = RenderGizmos::GetGridType();
    draw_gizmos.grid.mesh_handle = RenderGizmos::GetGridMesh()->GetAssetInfo().handle;
    draw_gizmos.transformation_gizmo.should_draw = RenderGizmos::GetShouldDrawTransformationGizmo();
    draw_gizmos.transformation_gizmo.local_to_world = RenderGizmos::GetTransformationGizmoTransformation();
    draw_gizmos.transformation_gizmo.mesh_handle = RenderGizmos::GetTransformationGizmoMesh()->GetAssetInfo().handle;
    draw_gizmos.should_draw_all_bounds = RenderGizmos::GetShouldDrawAllBounds();

    RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawGizmos);
    command.data = draw_gizmos;
  }

  //--------------------------------------------------------------
  void RenderFrame::DrawEditorUI() {
    RenderFrameCommandDrawEditorUI draw_editor_ui;

    RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawEditorUI);
    command.data = draw_editor_ui;
  }

  //--------------------------------------------------------------
  AsyncRequestResult &RenderFrame::AddAsyncRequestResult() {
    m_async_request_results.Resize(m_async_request_results.GetLength() + 1);
    return m_async_request_results.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameCommand &RenderFrame::CreateCommand(RenderFrameCommandType type) {
    m_commands.Resize(m_commands.GetLength() + 1);
    RenderFrameCommand &command = m_commands.GetLast();
    command.type = type;
    return command;
  }

}
