//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/frame/render_frame.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/ecs/system/systems/render_systems.hpp"
#include "hyperion/ecs/component/components/utilities/camera_utilities.hpp"
#include "hyperion/render/render_gizmos.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void RenderFrame::Clear() {
        m_context.Clear();
        m_commands.Clear();
        m_async_requests.Clear();
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
    void RenderFrame::ExecuteCommandBuffer(const RenderFrameCommandBuffer &command_buffer) {
        RenderFrameCommandExecuteCommandBuffer execute_command_buffer;
        execute_command_buffer.command_buffer = command_buffer;

        RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::ExecuteCommandBuffer);
        command.data = execute_command_buffer;
    }

    //--------------------------------------------------------------
    void RenderFrame::DrawMeshes(CullingResults &culling_results, DrawingParametes drawing_parameters) {
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
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
            }
        }

        RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawMeshes);
        command.data = draw_meshes;
    }

    //--------------------------------------------------------------
    void RenderFrame::DrawUI() {
        RenderFrameCommandDrawUI draw_ui;

        RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawUI);
        command.data = draw_ui;
    }

    //--------------------------------------------------------------
    void RenderFrame::DrawEditorGizmos() {
        RenderFrameCommandDrawEditorGizmos draw_editor_gizmos;
        draw_editor_gizmos.shader_id = AssetManager::GetShaderPrimitive(ShaderPrimitive::Gizmo)->GetAssetInfo().id;
        draw_editor_gizmos.grid.should_draw = RenderGizmos::GetShouldDrawGrid();
        draw_editor_gizmos.grid.local_to_world = Matrix4x4::Identity();
        draw_editor_gizmos.grid.type = RenderGizmos::GetGridType();
        draw_editor_gizmos.grid.mesh_id = RenderGizmos::GetGridMesh()->GetAssetInfo().id;
        draw_editor_gizmos.should_draw_all_bounds = RenderGizmos::GetShouldDrawAllBounds();

        RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawEditorGizmos);
        command.data = draw_editor_gizmos;
    }

    //--------------------------------------------------------------
    void RenderFrame::DrawEditorUI() {
        RenderFrameCommandDrawEditorUI draw_editor_ui;

        RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawEditorUI);
        command.data = draw_editor_ui;
    }

    //--------------------------------------------------------------
    AsyncRequest &RenderFrame::AddAsyncRequests() {
        m_async_requests.Resize(m_async_requests.GetLength() + 1);
        return m_async_requests.GetLast();
    }

    //--------------------------------------------------------------
    RenderFrameCommand &RenderFrame::CreateCommand(RenderFrameCommandType type) {
        m_commands.Resize(m_commands.GetLength() + 1);
        RenderFrameCommand &command = m_commands.GetLast();
        command.type = type;
        return command;
    }

}