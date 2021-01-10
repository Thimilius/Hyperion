#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

namespace Hyperion::Editor {

    class EditorApplication : public Application {
    public:
        EditorApplication(const ApplicationSettings &settings) : Application(settings) { }
    protected:
        void OnInit() override {

        }

        void OnUpdate(f32 delta_time) override {
            if ((Input::GetKey(KeyCode::LeftControl) || Input::GetKey(KeyCode::RightControl)) && Input::GetKeyDown(KeyCode::W)) {
                Exit();
            }
            if (Input::GetKeyDown(KeyCode::F1)) {
                GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
            }
        }

        void OnTick() override {
            String title = StringUtils::Format("Hyperion - FPS: {} ({:.2f}ms)", Time::GetFPS(), Time::GetFrameTime());
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