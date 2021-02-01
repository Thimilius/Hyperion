#pragma once

#include "hyperion/rendering/camera_data.hpp"

namespace Hyperion::Rendering {
    class IRenderDriver;
    class RenderEngine;
}

namespace Hyperion::Rendering {

    enum class RenderPipeline {
        Forward,
        Deferred,

        Custom
    };

    class RenderPipelineContext {
    public:
        const CameraData &GetCameraData() const { return m_camera_data; }
    private:
        CameraData m_camera_data;
    private:
        friend class Hyperion::Rendering::RenderEngine;
    };

    class IRenderPipeline {
    public:
        virtual ~IRenderPipeline() = default;

        virtual void Initialize(IRenderDriver *render_driver) = 0;
        virtual void Render(IRenderDriver *render_driver, const RenderPipelineContext &context) = 0;
        virtual void Shutdown(IRenderDriver *render_driver) = 0;
    };

}