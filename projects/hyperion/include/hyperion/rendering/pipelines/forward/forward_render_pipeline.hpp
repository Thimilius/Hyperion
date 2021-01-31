#pragma once

#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/shader.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/rendering/command_buffer.hpp"
#include "hyperion/rendering/pipelines/render_pipeline.hpp"

namespace Hyperion::Rendering {

    class ForwardRenderPipeline : public IRenderPipeline {
    public:
        void Initialize(IRenderDriver *render_driver) override;
        void Render(IRenderDriver *render_driver, const RenderPipelineContext &context) override;
        void Shutdown(IRenderDriver *render_driver) override;
    private:
        Shader *m_shader;
        Material *m_material;
        Font *m_font;
        Mesh *m_mesh;
        CommandBuffer *m_command_buffer;
        RenderTexture *m_render_texture;
    };

}