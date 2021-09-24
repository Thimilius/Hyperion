//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/frame/render_frame.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/render/render_gizmos.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void RenderFrame::Clear() {
        m_context.Clear();
        m_commands.Clear();
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
    void RenderFrame::DrawMeshes() {
        RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawMeshes);
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