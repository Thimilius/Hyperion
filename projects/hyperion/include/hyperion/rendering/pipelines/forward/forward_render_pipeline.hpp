#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"
#include "hyperion/entity/components/ui/canvas.hpp"
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

        void DrawWorld(IRenderDriver *render_driver, World *world, const Rendering::CameraData &camera_data);
        void DrawMeshRenderer(IRenderDriver *render_driver, MeshRenderer *mesh_renderer, Material *overwrite_material = nullptr);
        void DrawCanvas(IRenderDriver *render_driver, Canvas *canvas);
    private:
        void SetUICameraData(IRenderDriver *render_driver);
    private:
        bool m_is_primary;
        RenderTexture *m_render_texture;
    };

}