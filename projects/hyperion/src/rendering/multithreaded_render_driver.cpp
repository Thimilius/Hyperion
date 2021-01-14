#include "hyppch.hpp"

#include "hyperion/rendering/multithreaded_render_driver.hpp"

#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    void MultithreadedRenderDriver::Clear(ClearFlags clear_flags, Color color) {
        RenderCommandClear *clear = RenderEngine::GetCommandQueue().Allocate<RenderCommandClear>(RenderCommandType::Clear);
        clear->clear_flags = clear_flags;
        clear->color = color;
    }

    void MultithreadedRenderDriver::CreateMesh(ResourceId id, const Vector<VertexMesh> &vertices, const Vector<u32> &indices) {
        RenderCommandCreateMesh *create_mesh = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateMesh>(RenderCommandType::CreateMesh);
        create_mesh->id = id;
        create_mesh->verticies = vertices;
        create_mesh->indicies = indices;
    }

}