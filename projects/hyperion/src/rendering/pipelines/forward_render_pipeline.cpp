#include "hyppch.hpp"

#include "hyperion/rendering/pipelines/forward_render_pipeline.hpp"

#include "hyperion/core/app/time.hpp"
#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    void ForwardRenderPipeline::Render() {
        Color color = Color::Cyan();
        f32 value = Math::Sin(Time::GetTime() * 2.0f) / 2.0f + 0.5f;
        color *= value;
        RenderEngine::GetRenderDriver()->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, color);
    }

}