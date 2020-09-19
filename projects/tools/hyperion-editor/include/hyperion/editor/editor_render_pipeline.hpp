#pragma once

#include <hyperion/rendering/render_pipeline.hpp>

namespace Hyperion::Editor {

    class EditorRenderPipeline : public Rendering::RenderPipeline {
    public:
        void Render() override;
    };

}