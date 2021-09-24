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
        m_culling_storages.Clear();
    }

    //--------------------------------------------------------------
    const CullingStorage &RenderFrame::GetCullingStorage(uint32 index) const {
        HYP_ASSERT(index < m_culling_storages.GetLength());

        return m_culling_storages[index];
    }

    //--------------------------------------------------------------
    CullingResults RenderFrame::Cull(CullingParameters parameters) {
        HYP_PROFILE_SCOPE("RenderFrame.Cull");

        Array<Plane> frustum_planes = CameraUtilities::ExtractFrustumPlanes(parameters.matrix);

        CullingResults result;
        result.index = static_cast<uint32>(m_culling_storages.GetLength());

        m_culling_storages.Resize(m_culling_storages.GetLength() + 1);
        CullingStorage &storage = m_culling_storages.GetLast();

        uint32 index = 0;
        for (const RenderFrameContextObjectMesh &object : m_context.GetMeshObjects()) {
            bool8 is_inside_frustum = CameraUtilities::IsInsideFrustum(frustum_planes, object.bounds);
            bool8 is_in_layer = (object.layer_mask & parameters.mask) == object.layer_mask;

            if (is_inside_frustum && is_in_layer) {
                storage.indices.Add(index);
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
    void RenderFrame::DrawMeshes(CullingResults culling_results, DrawingParametes drawing_parameters) {
        RenderFrameCommandDrawMeshes draw_meshes;
        draw_meshes.culling_results = culling_results;
        draw_meshes.drawing_parameters = drawing_parameters;

        RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawMeshes);
        command.data = draw_meshes;
    }

    //--------------------------------------------------------------
    void RenderFrame::DrawGizmos() {
        RenderFrameCommandDrawGizmos draw_gizmos;
        draw_gizmos.shader_id = AssetManager::GetShaderPrimitive(ShaderPrimitive::Gizmo)->GetAssetInfo().id;
        draw_gizmos.grid.should_draw = RenderGizmos::GetShouldDrawGrid();
        draw_gizmos.grid.local_to_world = Matrix4x4::Identity();
        draw_gizmos.grid.type = RenderGizmos::GetGridType();
        draw_gizmos.grid.mesh_id = RenderGizmos::GetGridMesh()->GetAssetInfo().id;
        draw_gizmos.should_draw_all_bounds = RenderGizmos::GetShouldDrawAllBounds();

        RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawGizmos);
        command.data = draw_gizmos;
    }

    //--------------------------------------------------------------
    RenderFrameCommand &RenderFrame::CreateCommand(RenderFrameCommandType type) {
        m_commands.Resize(m_commands.GetLength() + 1);
        RenderFrameCommand &command = m_commands.GetLast();
        command.type = type;
        return command;
    }

}