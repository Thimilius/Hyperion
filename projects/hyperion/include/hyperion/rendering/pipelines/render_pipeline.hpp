#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"
#include "hyperion/rendering/data/camera_data.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
    class IRenderDriver;
    class RenderEngine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class RenderPipeline {
        Forward,
        Deferred,

        Custom
    };

    class RenderPipelineContext {
    public:
        const CameraData &GetCameraData() const { return m_camera_data; }
        const Vector<MeshRenderer *> &GetMeshRenderers() const { return m_mesh_renderers; }
    private:
        RenderPipelineContext(const CameraData &camera_data, const Vector<MeshRenderer *> &mesh_renderers);
    private:
        CameraData m_camera_data;
        const Vector<MeshRenderer *> &m_mesh_renderers;
    private:
        friend class Hyperion::Rendering::RenderEngine;
    };

    class IRenderPipeline {
    public:
        virtual ~IRenderPipeline() = default;
    public:
        virtual void Initialize(IRenderDriver *render_driver) = 0;
        virtual void Render(IRenderDriver *render_driver) = 0;
        virtual void Shutdown(IRenderDriver *render_driver) = 0;
    };

}