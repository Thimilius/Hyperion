//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/core/app/application.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/io/file_system.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/core/serialization/json_serializer.hpp>
#include <hyperion/core/system/engine.hpp>
#include <hyperion/entity/world_manager.hpp>
#include <hyperion/scripting/scripting_engine.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/rendering/editor_render_pipeline.hpp"
#include "hyperion/editor/world_view/editor_world_view.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorApplication::OnInitialize() {
        EditorWorldView::Initialize();

        World *world = WorldManager::CreateWorld("Hello world");
        world->GetEnvironment().ambient_light.color = Color::Blue();
        world->GetEnvironment().ambient_light.intensity = 0.45f;

        Entity *entity = Entity::Create("Entity", Vec3::Zero(), Quaternion::Identity(), nullptr, world);
        entity->SetLayer(LayerMask::Layer17);

        JsonSerializer serializer;

        String world_json = serializer.Serialize(world).Unwrap();
        World *new_world = serializer.DeserializeRaw<World>(world_json).Unwrap();

        String entity_json = serializer.Serialize(entity).Unwrap();
        Entity *new_entity = serializer.DeserializeRaw<Entity>(entity_json).Unwrap();
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

    //--------------------------------------------------------------
    void EditorApplication::OnTick() {
        String engine_mode = Engine::GetMode() == EngineMode::Editor ? "Paused" : "Playing";
        float32 native_memory = static_cast<float32>(MemoryStats::GetGlobalMemory()) / 1000.0f / 1000.0f;
        float32 managed_memory = static_cast<float32>(Scripting::ScriptingEngine::GetMemoryUsage()) / 1000.0f / 1000.0f;
        String title = StringUtils::Format("Hyperion - Playstate: {} - FPS: {} ({:.2f}ms) - Native Memory: {:.2f}MB - Managed Memory: {:.2f}MB", engine_mode, Time::GetFPS(), Time::GetFrameTime(), native_memory, managed_memory);
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