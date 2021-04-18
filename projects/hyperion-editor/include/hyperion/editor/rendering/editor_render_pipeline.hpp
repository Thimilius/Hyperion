#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/rendering/pipelines/render_pipeline.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorRenderPipeline : public Rendering::IRenderPipeline {
    public:
        void Initialize() override;
        void Render() override;
        void Shutdown() override;
    };

}