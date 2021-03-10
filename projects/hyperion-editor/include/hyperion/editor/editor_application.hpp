#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/application.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/rendering/editor_render_pipeline.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorApplication : public Application {
    public:
        EditorApplication(const ApplicationSettings &settings);
    public:
        inline static EditorRenderPipeline *GetRenderPipeline() { return s_editor_render_pipeline; }
    protected:
        void OnInitialize() override;
        void OnUpdate(float32 delta_time) override;
    private:
        inline static EditorRenderPipeline *s_editor_render_pipeline;
    };

}