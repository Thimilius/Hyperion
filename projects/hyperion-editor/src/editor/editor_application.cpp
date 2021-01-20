#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

#include "hyperion/core/memory.hpp"

namespace Hyperion::Editor {

    class EditorApplication : public Application {
    public:
        EditorApplication(const ApplicationSettings &settings) : Application(settings) { }
    protected:
        void OnInit() override {
            
        }

        void OnUpdate(float32 delta_time) override {
            if ((Input::GetKey(KeyCode::LeftControl) || Input::GetKey(KeyCode::RightControl)) && Input::GetKeyDown(KeyCode::W)) {
                Exit();
            }
            if (Input::GetKeyDown(KeyCode::F1)) {
                GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
            }
        }

        void OnTick() override {
            float32 memory = static_cast<float32>(MemoryStats::GetGlobalMemory()) / 1000.0f / 1000.0f;
            String title = StringUtils::Format("Hyperion - FPS: {} ({:.2f}ms) - Memory: {:.2f}MB", Time::GetFPS(), Time::GetFrameTime(), memory);
            GetWindow()->SetTitle(title);
        }

        void OnEvent(Event &event) override {
            
        }

        void OnShutdown() override {

        }
    };

}

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings::FromJsonFile("app.json");
    settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
    return new Hyperion::Editor::EditorApplication(settings);
}