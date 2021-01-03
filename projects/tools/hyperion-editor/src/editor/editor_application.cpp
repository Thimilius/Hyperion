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

        }

        void OnTick() override {

        }

        void OnEvent(Event &event) override {

        }
    };

}

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings::FromJsonFile("app.json");
    return new Hyperion::Editor::EditorApplication(settings);
}