#include "hyppch.hpp"

#include "hyperion/rendering/commands/multithreaded_render_driver.hpp"

#include "hyperion/core/threading/synchronization.hpp"
#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    void MultithreadedRenderDriver::Clear(ClearFlags clear_flags, Color color) {
        RenderCommandClear *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandClear>(RenderCommandType::Clear);
        command->clear_flags = clear_flags;
        command->color = color;
    }

    void MultithreadedRenderDriver::SetViewport(const Viewport &viewport) {
        RenderCommandViewport *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandViewport>(RenderCommandType::SetViewport);
        command->viewport = viewport;
    }

    void MultithreadedRenderDriver::SetRasterizerState(const RasterizerState &rasterizer_state) {
        RenderCommandSetRasterizerState *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandSetRasterizerState>(RenderCommandType::SetRasterizerState);
        command->rasterizer_state = rasterizer_state;
    }

    void MultithreadedRenderDriver::CreateShader(ResourceId shader_id, const ShaderDescriptor &descriptor) {
        uint64 extra_size = descriptor.source_vertex.size + descriptor.source_fragment.size;
        RenderCommandCreateShader *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateShader>(RenderCommandType::CreateShader, extra_size);
        command->shader_id = shader_id;
        command->descriptor = descriptor;

        uint8 *data = reinterpret_cast<uint8 *>(command + 1);
        std::memcpy(data, descriptor.source_vertex.data, descriptor.source_vertex.size);
        data += descriptor.source_vertex.size;
        std::memcpy(data, descriptor.source_fragment.data, descriptor.source_fragment.size);
    }

    void MultithreadedRenderDriver::DestroyShader(ResourceId shader_id) {
        RenderCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::DestroyShader);
        command->id = shader_id;
    }

    void MultithreadedRenderDriver::CreateTexture(ResourceId texture_id, const TextureDescriptor &descriptor) {
        uint64 extra_size = descriptor.pixels.size;
        RenderCommandCreateTexture *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateTexture>(RenderCommandType::CreateTexture, extra_size);
        command->texture_id = texture_id;
        command->descriptor = descriptor;

        uint8 *data = reinterpret_cast<uint8 *>(command + 1);
        std::memcpy(data, descriptor.pixels.data, descriptor.pixels.size);
    }

    void MultithreadedRenderDriver::GetTextureData(ResourceId texture_id, Vector<uint8> &data) {
        // NOTE: Getting the data of a texture is an immediate render command and needs to be handled appropriately.
        ImmediateRenderCommandGetTextureData command;
        command.texture_id = texture_id;
        command.data = &data;

        ImmediateRenderCommand &immediate_command = RenderEngine::GetImmediateCommand();
        immediate_command.type = ImmediateRenderCommandType::GetTextureData;
        immediate_command.command = command;

        RenderEngine::SetImmediateCommandPending();
        Synchronization::WaitForImmediateCommandDone();
    }

    void MultithreadedRenderDriver::DestroyTexture(ResourceId texture_id) {
        RenderCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::DestroyTexture);
        command->id = texture_id;
    }

    void MultithreadedRenderDriver::CreateMaterial(ResourceId material_id, const MaterialDescriptor &descriptor) {
        RenderCommandCreateMaterial *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateMaterial>(RenderCommandType::CreateMaterial);
        command->material_id = material_id;
        command->descriptor = descriptor;
    }

    void MultithreadedRenderDriver::SetMaterialProperty(ResourceId material_id, const MaterialProperty &property) {
        uint64 extra_size = property.name.size;
        RenderCommandSetMaterialProperty *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandSetMaterialProperty>(RenderCommandType::SetMaterialProperty, extra_size);
        command->material_id = material_id;
        command->property = property;

        uint8 *data = reinterpret_cast<uint8 *>(command + 1);
        std::memcpy(data, property.name.data, property.name.size);
    }

    void MultithreadedRenderDriver::DestroyMaterial(ResourceId id) {
        RenderCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::DestroyMaterial);
        command->id = id;
    }

    void MultithreadedRenderDriver::CreateMesh(ResourceId mesh_id, const MeshDescriptor &descriptor) {
        uint64 extra_size = descriptor.sub_meshes.size + descriptor.vertex_format.attributes.size + descriptor.vertices.size + descriptor.indices.size;
        RenderCommandCreateMesh *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateMesh>(RenderCommandType::CreateMesh, extra_size);
        command->mesh_id = mesh_id;
        command->descriptor = descriptor;

        uint8 *data = reinterpret_cast<uint8 *>(command + 1);
        std::memcpy(data, descriptor.sub_meshes.data, descriptor.sub_meshes.size);
        data += descriptor.sub_meshes.size;
        std::memcpy(data, descriptor.vertex_format.attributes.data, descriptor.vertex_format.attributes.size);
        data += descriptor.vertex_format.attributes.size;
        std::memcpy(data, descriptor.vertices.data, descriptor.vertices.size);
        data += descriptor.vertices.size;
        std::memcpy(data, descriptor.indices.data, descriptor.indices.size);
    }

    void MultithreadedRenderDriver::DrawMesh(ResourceId mesh_id, ResourceId material_id, uint32 sub_mesh_index) {
        RenderCommandDrawMesh *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandDrawMesh>(RenderCommandType::DrawMesh);
        command->mesh_id = mesh_id;
        command->material_id = material_id;
        command->sub_mesh_index = sub_mesh_index;
    }

    void MultithreadedRenderDriver::DestroyMesh(ResourceId mesh_id) {
        RenderCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::DestroyMesh);
        command->id = mesh_id;
    }

}