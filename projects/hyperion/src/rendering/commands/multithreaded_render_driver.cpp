#include "hyppch.hpp"

#include "hyperion/rendering/commands/multithreaded_render_driver.hpp"

#include "hyperion/core/threading/synchronization.hpp"
#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    void MultithreadedRenderDriver::Clear(ClearFlags clear_flags, Color color) {
        RenderCommandClear *render_command = RenderEngine::GetCommandQueue().Allocate<RenderCommandClear>(RenderCommandType::Clear);
        render_command->clear_flags = clear_flags;
        render_command->color = color;
    }

    void MultithreadedRenderDriver::SetViewport(const Viewport &viewport) {
        RenderCommandViewport *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandViewport>(RenderCommandType::SetViewport);
        command->viewport = viewport;
    }

    void MultithreadedRenderDriver::SetRasterizerState(const RasterizerState &rasterizer_state) {
        RenderCommandSetRasterizerState *render_command = RenderEngine::GetCommandQueue().Allocate<RenderCommandSetRasterizerState>(RenderCommandType::SetRasterizerState);
        render_command->rasterizer_state = rasterizer_state;
    }

    void MultithreadedRenderDriver::CreateShader(ResourceId shader_id, const ShaderDescriptor &descriptor) {
        uint64 extra_size = descriptor.source_vertex.size + descriptor.source_fragment.size;
        RenderCommandCreateShader *render_command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateShader>(RenderCommandType::CreateShader, extra_size);
        render_command->shader_id = shader_id;
        render_command->descriptor = descriptor;

        RenderCommandQueueHelper<RenderCommandCreateShader> helper(render_command);
        helper.Write(descriptor.source_vertex);
        helper.Write(descriptor.source_fragment);
    }

    void MultithreadedRenderDriver::DestroyShader(ResourceId shader_id) {
        RenderCommandId *render_command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::DestroyShader);
        render_command->id = shader_id;
    }

    void MultithreadedRenderDriver::CreateTexture(ResourceId texture_id, const TextureDescriptor &descriptor) {
        uint64 extra_size = descriptor.pixels.size;
        RenderCommandCreateTexture *render_command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateTexture>(RenderCommandType::CreateTexture, extra_size);
        render_command->texture_id = texture_id;
        render_command->descriptor = descriptor;

        RenderCommandQueueHelper<RenderCommandCreateTexture> helper(render_command);
        helper.Write(descriptor.pixels);
    }

    void MultithreadedRenderDriver::GetTextureData(ResourceId texture_id, Vector<uint8> &data) {
        // NOTE: Getting the data of a texture is an immediate render command and needs to be handled appropriately.
        ImmediateRenderCommandGetTextureData render_command = { };
        render_command.texture_id = texture_id;
        render_command.data = &data;

        ImmediateRenderCommand &immediate_command = RenderEngine::GetImmediateCommand();
        immediate_command.type = ImmediateRenderCommandType::GetTextureData;
        immediate_command.command = render_command;

        RenderEngine::SetImmediateCommandPending();
        Synchronization::WaitForImmediateCommandDone();
    }

    void MultithreadedRenderDriver::DestroyTexture(ResourceId texture_id) {
        RenderCommandId *render_command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::DestroyTexture);
        render_command->id = texture_id;
    }

    void MultithreadedRenderDriver::CreateMaterial(ResourceId material_id, const MaterialDescriptor &descriptor) {
        RenderCommandCreateMaterial *render_command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateMaterial>(RenderCommandType::CreateMaterial);
        render_command->material_id = material_id;
        render_command->descriptor = descriptor;
    }

    void MultithreadedRenderDriver::SetMaterialProperty(ResourceId material_id, const MaterialProperty &property) {
        RenderCommandSetMaterialProperty *render_command = RenderEngine::GetCommandQueue().Allocate<RenderCommandSetMaterialProperty>(RenderCommandType::SetMaterialProperty);
        render_command->material_id = material_id;
        render_command->property = property;
    }

    void MultithreadedRenderDriver::DestroyMaterial(ResourceId id) {
        RenderCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::DestroyMaterial);
        command->id = id;
    }

    void MultithreadedRenderDriver::CreateMesh(ResourceId mesh_id, const MeshDescriptor &descriptor) {
        uint64 extra_size = descriptor.sub_meshes.size + descriptor.vertex_format.attributes.size + descriptor.vertices.size + descriptor.indices.size;
        RenderCommandCreateMesh *render_command = RenderEngine::GetCommandQueue().Allocate<RenderCommandCreateMesh>(RenderCommandType::CreateMesh, extra_size);
        render_command->mesh_id = mesh_id;
        render_command->descriptor = descriptor;

        RenderCommandQueueHelper helper(render_command);
        helper.Write(descriptor.sub_meshes);
        helper.Write(descriptor.vertex_format.attributes);
        helper.Write(descriptor.vertices);
        helper.Write(descriptor.indices);
    }

    void MultithreadedRenderDriver::DrawMesh(ResourceId mesh_id, ResourceId material_id, uint32 sub_mesh_index) {
        RenderCommandDrawMesh *render_command = RenderEngine::GetCommandQueue().Allocate<RenderCommandDrawMesh>(RenderCommandType::DrawMesh);
        render_command->mesh_id = mesh_id;
        render_command->material_id = material_id;
        render_command->sub_mesh_index = sub_mesh_index;
    }

    void MultithreadedRenderDriver::DestroyMesh(ResourceId mesh_id) {
        RenderCommandId *render_command = RenderEngine::GetCommandQueue().Allocate<RenderCommandId>(RenderCommandType::DestroyMesh);
        render_command->id = mesh_id;
    }

}