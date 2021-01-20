#include "hyppch.hpp"

#include "hyperion/rendering/multithreaded_render_driver.hpp"

#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    void MultithreadedRenderDriver::Clear(ClearFlags clear_flags, Color color) {
        RenderCommandClear *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandClear>(RenderCommandType::Clear);
        command->clear_flags = clear_flags;
        command->color = color;
    }

    void MultithreadedRenderDriver::Viewport(const Rendering::Viewport &viewport) {
        RenderCommandViewport *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandViewport>(RenderCommandType::Viewport);
        command->viewport = viewport;
    }

    void MultithreadedRenderDriver::SetRasterizerState(const RasterizerState &rasterizer_state) {
        RenderCommandSetRasterizerState *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandSetRasterizerState>(RenderCommandType::SetRasterizerState);
        command->rasterizer_state = rasterizer_state;
    }

    void MultithreadedRenderDriver::CreateShader(ResourceId id, const ShaderDescriptor &descriptor) {
        uint64 extra_size = descriptor.source_vertex.size + descriptor.source_fragment.size;
        RenderCommandCreateShader *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateShader>(RenderCommandType::CreateShader, extra_size);
        command->id = id;
        command->descriptor = descriptor;

        uint8 *data = reinterpret_cast<uint8 *>(command + 1);
        std::memcpy(data, descriptor.source_vertex.data, descriptor.source_vertex.size);
        data += descriptor.source_vertex.size;
        std::memcpy(data, descriptor.source_fragment.data, descriptor.source_fragment.size);
    }

    void MultithreadedRenderDriver::FreeShader(ResourceId id) {
        RenderCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::FreeShader);
        command->id = id;
    }

    void MultithreadedRenderDriver::CreateMesh(ResourceId id, const MeshDescriptor &descriptor) {
        uint64 extra_size = descriptor.sub_meshes.size + descriptor.vertices.size + descriptor.indices.size;
        RenderCommandCreateMesh *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateMesh>(RenderCommandType::CreateMesh, extra_size);
        command->id = id;
        command->descriptor = descriptor;

        uint8 *data = reinterpret_cast<uint8 *>(command + 1);
        std::memcpy(data, descriptor.sub_meshes.data, descriptor.sub_meshes.size);
        data += descriptor.sub_meshes.size;
        std::memcpy(data, descriptor.vertices.data, descriptor.vertices.size);
        data += descriptor.vertices.size;
        std::memcpy(data, descriptor.indices.data, descriptor.indices.size);
    }

    void MultithreadedRenderDriver::FreeMesh(ResourceId id) {
        RenderCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::FreeMesh);
        command->id = id;
    }

    void MultithreadedRenderDriver::CreateTexture(ResourceId id, const TextureDescriptor &descriptor) {
        uint64 extra_size = descriptor.pixels.size;
        RenderCommandCreateTexture *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateTexture>(RenderCommandType::CreateTexture, extra_size);
        command->id = id;
        command->descriptor = descriptor;

        uint8 *data = reinterpret_cast<uint8 *>(command + 1);
        std::memcpy(data, descriptor.pixels.data, descriptor.pixels.size);
    }

    void MultithreadedRenderDriver::FreeTexture(ResourceId id) {
        RenderCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::FreeTexture);
        command->id = id;
    }

    void MultithreadedRenderDriver::DrawIndexed(ResourceId shader_id, ResourceId mesh_id) {
        RenderCommandDrawIndexed *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandDrawIndexed>(RenderCommandType::DrawIndexed);
        command->shader_id = shader_id;
        command->mesh_id = mesh_id;
    }

}