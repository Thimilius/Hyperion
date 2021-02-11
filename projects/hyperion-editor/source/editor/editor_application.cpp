#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/scripting/scripting_engine.hpp>

#include "hyperion/editor/rendering/editor_render_pipeline.hpp"
#include "hyperion/editor/world/editor_world.hpp"

namespace Hyperion::Editor {

    class EditorApplication : public Application {
    public:
        EditorApplication(const ApplicationSettings &settings) : Application(settings) { }
    protected:
        void OnInitialize() override {
            EditorWorld::Initialize();
        }

        void OnUpdate(float32 delta_time) override {
            if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
                Exit();
            }
            if (Input::IsKeyDown(KeyCode::F1)) {
                GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
            }

            EditorWorld::Update(delta_time);
        }

        void OnTick() override {
            float32 native_memory = static_cast<float32>(MemoryStats::GetGlobalMemory()) / 1000.0f / 1000.0f;
            float32 managed_memory = static_cast<float32>(Scripting::ScriptingEngine::GetMemoryUsage()) / 1000.0f / 1000.0f;
            String title = StringUtils::Format("Hyperion - FPS: {} ({:.2f}ms) - Native Memory: {:.2f}MB - Managed Memory: {:.2f}MB", Time::GetFPS(), Time::GetFrameTime(), native_memory, managed_memory);
            GetWindow()->SetTitle(title);
        }
    };

}

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings::FromJsonFile("app.json");
    settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
    settings.render.pipeline = Rendering::RenderPipeline::Custom;
    settings.render.custom_pipeline = new Editor::EditorRenderPipeline();
    return new Hyperion::Editor::EditorApplication(settings);
}