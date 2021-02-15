#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/assets/font.hpp>
#include <hyperion/assets/material.hpp>
#include <hyperion/assets/mesh.hpp>
#include <hyperion/assets/shader.hpp>
#include <hyperion/assets/texture.hpp>
#include <hyperion/core/color.hpp>
#include <hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorRenderPipeline : public Rendering::IRenderPipeline {
    public:
        void Initialize(Rendering::IRenderDriver *render_driver) override;
        void Render(Rendering::IRenderDriver *render_driver) override;
        void Shutdown(Rendering::IRenderDriver *render_driver) override;
    private:
        Rendering::ForwardRenderPipeline *m_forward_render_pipeline;
        RenderTexture *m_render_texture;
    };

}