#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/application.hpp>
#include <hyperion/core/system/engine_mode.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/rendering/editor_render_pipeline.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorApplication : public Application {
    public:
        EditorApplication(const ApplicationSettings &settings);
    public:
        inline static World *GetEditingWorld() { return s_editing_world; }
        inline static void SetEditingWorld(World *editing_world) { s_editing_world = editing_world; }

        inline static EditorRenderPipeline *GetRenderPipeline() { return s_editor_render_pipeline; }
    protected:
        void OnInitialize() override;
        void OnUpdate(float32 delta_time) override;
    private:
        static void EnterEngineMode(EngineMode engine_mode);
        static void ReloadEditingWorld();
    private:
        inline static World *s_editing_world = nullptr;

        inline static EditorRenderPipeline *s_editor_render_pipeline;
    };

}