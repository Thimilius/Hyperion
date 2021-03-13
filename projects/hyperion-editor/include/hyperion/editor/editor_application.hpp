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
        inline static World *GetLoadedWorld() { return s_loaded_world; }
        inline static void SetLoadedWorld(World *loaded_world) { s_loaded_world = loaded_world; }

        inline static EditorRenderPipeline *GetRenderPipeline() { return s_editor_render_pipeline; }
    protected:
        void OnInitialize() override;
        void OnUpdate(float32 delta_time) override;
    private:
        inline static World *s_loaded_world;

        inline static EditorRenderPipeline *s_editor_render_pipeline;
    };

}