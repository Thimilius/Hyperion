#include "hyppch.hpp"

#include "hyperion/rendering/pipelines/forward_render_pipeline.hpp"

#include "hyperion/core/app/time.hpp"
#include "hyperion/rendering/render_operation.hpp"

namespace Hyperion::Rendering {

    void ForwardRenderPipeline::Render() {
        Color color = Color::Cyan();
        f32 time = static_cast<f32>(Time::GetTime());
        f32 value = Math::Sin(time * 2.0f) / 2.0f + 0.5f;
        color *= value;
        RenderOperation::Clear(ClearMask::Color | ClearMask::Depth | ClearMask::Stencil, color);
    }

}