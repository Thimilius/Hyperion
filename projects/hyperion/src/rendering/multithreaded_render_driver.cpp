#include "hyppch.hpp"

#include "hyperion/rendering/multithreaded_render_driver.hpp"

#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    void MultithreadedRenderDriver::Clear(ClearFlags clear_flags, Color color) {
        RenderCommandClear *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandClear>(RenderCommandType::Clear);
        command->clear_flags = clear_flags;
        command->color = color;
    }

    void MultithreadedRenderDriver::CreateShader(ResourceId id, const String &vertex, const String &fragment) {
        RenderCommandCreateShader *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateShader>(RenderCommandType::CreateShader);
        command->id = id;
        command->vertex = vertex;
        command->fragment = fragment;
    }

    void MultithreadedRenderDriver::FreeShader(ResourceId id) {
        RenderCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::FreeShader);
        command->id = id;
    }

    void MultithreadedRenderDriver::CreateMesh(ResourceId id, const Vector<VertexMesh> &vertices, const Vector<u32> &indices) {
        RenderCommandCreateMesh *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateMesh>(RenderCommandType::CreateMesh);
        command->id = id;
        command->verticies = vertices;
        command->indicies = indices;
    }

    void MultithreadedRenderDriver::FreeMesh(ResourceId id) {
        RenderCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::FreeMesh);
        command->id = id;
    }

    void MultithreadedRenderDriver::DrawIndexed(ResourceId shader_id, ResourceId mesh_id) {
        RenderCommandDrawIndexed *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandDrawIndexed>(RenderCommandType::DrawIndexed);
        command->shader_id = shader_id;
        command->mesh_id = mesh_id;
    }

}