//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/core/app/application.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/core/system/engine.hpp>
#include <hyperion/scripting/scripting_engine.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/rendering/editor_render_pipeline.hpp"
#include "hyperion/editor/entity/editor_world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorApplication::OnInitialize() {
        EditorWorld::Initialize();
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

        EditorWorld::Update(delta_time);
    }

    //--------------------------------------------------------------
    void EditorApplication::OnTick() {
        float32 native_memory = static_cast<float32>(MemoryStats::GetGlobalMemory()) / 1000.0f / 1000.0f;
        float32 managed_memory = static_cast<float32>(Scripting::ScriptingEngine::GetMemoryUsage()) / 1000.0f / 1000.0f;
        String title = StringUtils::Format("Hyperion - FPS: {} ({:.2f}ms) - Native Memory: {:.2f}MB - Managed Memory: {:.2f}MB", Time::GetFPS(), Time::GetFrameTime(), native_memory, managed_memory);
        GetWindow()->SetTitle(title);
    }

}

//--------------------------------------------------------------
Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings::FromJsonFile("app.json");
    settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
    settings.render.pipeline = Rendering::RenderPipeline::Custom;
    settings.render.custom_pipeline = new Editor::EditorRenderPipeline();
    return new Hyperion::Editor::EditorApplication(settings);
}