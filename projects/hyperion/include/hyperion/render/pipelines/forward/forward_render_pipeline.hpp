#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/frame/render_frame.hpp"
#include "hyperion/render/pipelines/render_pipeline.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Material;
    class Mesh;
    class Shader;

    namespace Rendering {
        struct RenderFrameContextCamera;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct Light {
        Color color;
        Vector3 direction;
        float32 intensity;
        Vector3 position;
        float32 range;
        float32 spot_inner_radius;
        float32 spot_outer_radius;

        float32 padding[2];
    };

    struct LightingBuffer {
        Color ambient_color;

        Light main_light;

        Light point_lights[128];
    };

    class ForwardRenderPipeline : public IRenderPipeline {
    public:
        void Initialize() override;
        void Render(RenderFrame *render_view) override;
        void Shutdown() override;
    private:
        void SetupLighting(RenderFrameContext &context, RenderFrameCommandBuffer &command_buffer);
        void CopyFrameLightToLight(const RenderFrameContextLight &frame_light, Light &light);
    };

}