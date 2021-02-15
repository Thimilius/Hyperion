#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"
#include "hyperion/rendering/pipelines/render_pipeline.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class ForwardRenderPipeline : public IRenderPipeline {
    public:
        ForwardRenderPipeline(bool is_primary);
    public:
        void Initialize(IRenderDriver *render_driver) override;
        void Render(IRenderDriver *render_driver) override;
        void Shutdown(IRenderDriver *render_driver) override;

        void RenderWorld(IRenderDriver *render_driver, World *world, const Rendering::CameraData &camera_data);
    private:
        bool m_is_primary;
        RenderTexture *m_render_texture;
    };

}