#pragma once

#include <hyperion/assets/material.hpp>
#include <hyperion/assets/mesh.hpp>
#include <hyperion/assets/shader.hpp>
#include <hyperion/assets/texture.hpp>
#include <hyperion/core/color.hpp>
#include <hyperion/rendering/pipelines/render_pipeline.hpp>

namespace Hyperion::Editor {

    class EditorRenderPipeline : public Rendering::IRenderPipeline {
    public:
        void Initialize(Rendering::IRenderDriver *render_driver) override;
        void Render(Rendering::IRenderDriver *render_driver, const Rendering::RenderPipelineContext &context) override;
        void Shutdown(Rendering::IRenderDriver *render_driver) override;
    private:
        Material *m_material;
        Shader *m_shader;
        Texture2D *m_texture;
        Mesh *m_mesh;
        RenderTexture *m_render_texture;
    };

}