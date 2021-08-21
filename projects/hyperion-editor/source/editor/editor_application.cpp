//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/core/app/application.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/io/file_system.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/core/system/engine.hpp>
#include <hyperion/entity/world_manager.hpp>
#include <hyperion/scripting/scripting_engine.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_selection.hpp"
#include "hyperion/editor/rendering/editor_render_pipeline.hpp"
#include "hyperion/editor/world_view/editor_world_view.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorApplication::OnSetup(ApplicationSettings &settings) {
        s_editor_render_pipeline = new Editor::EditorRenderPipeline();

        settings.render.graphics_backend = Graphics::GraphicsBackend::OpenGL;
        settings.render.pipeline = Rendering::RenderPipeline::Custom;
        settings.render.custom_pipeline = s_editor_render_pipeline;
    }

    //--------------------------------------------------------------
    void EditorApplication::OnInitialize() {
        EditorWorldView::Initialize();

        ReloadEditingWorld();
    }

    //--------------------------------------------------------------
    void EditorApplication::OnUpdate(float32 delta_time) {
        if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
            Exit();
        }
        if (Input::IsKeyDown(KeyCode::F1)) {
            GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
        }
        if (Input::IsKeyDown(KeyCode::F5)) {
            EnterEngineMode(Engine::GetMode() == EngineMode::Editor ? EngineMode::EditorRuntime : EngineMode::Editor);
        }

        EditorWorldView::Update(delta_time);
    }

    //--------------------------------------------------------------
    void EditorApplication::EnterEngineMode(EngineMode engine_mode) {
        Engine::SetMode(engine_mode);

        // When going back to normal editor mode, we want to reset the complete world state.
        if (engine_mode == EngineMode::Editor) {
            ReloadEditingWorld();
        }
    }

    //--------------------------------------------------------------
    void EditorApplication::ReloadEditingWorld() {
        World *new_world = WorldManager::CreateWorld();
        WorldManager::SetActiveWorld(new_world);
        WorldManager::DestroyWorld(s_editing_world);
        s_editing_world = new_world;
    }

}

//--------------------------------------------------------------
Hyperion::Application *Hyperion::CreateApplication() {
    return new Hyperion::Editor::EditorApplication();
}