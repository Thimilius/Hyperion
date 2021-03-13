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
#include "hyperion/editor/rendering/editor_render_pipeline.hpp"
#include "hyperion/editor/world_view/editor_world_view.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    EditorApplication::EditorApplication(const ApplicationSettings &settings) : Application(settings) {
        s_editor_render_pipeline = static_cast<EditorRenderPipeline *>(settings.render.custom_pipeline);
    }

    //--------------------------------------------------------------
    void EditorApplication::OnInitialize() {
        EditorWorldView::Initialize();

        s_loaded_world = WorldManager::CreateWorld();

        Entity::CreatePrimitive(EntityPrimitive::Cube, Vec3::Zero(), Quaternion::Identity(), nullptr, s_loaded_world);
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
            Engine::SetMode(Engine::GetMode() == EngineMode::Editor ? EngineMode::EditorRuntime : EngineMode::Editor);
        }

        EditorWorldView::Update(delta_time);
    }

}

//--------------------------------------------------------------
Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings();
    settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
    settings.render.pipeline = Rendering::RenderPipeline::Custom;
    settings.render.custom_pipeline = new Editor::EditorRenderPipeline();
    return new Hyperion::Editor::EditorApplication(settings);
}