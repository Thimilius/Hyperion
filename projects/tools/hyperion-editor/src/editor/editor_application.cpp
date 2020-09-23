#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

#include "hyperion/editor/editor_engine.hpp"
#include "hyperion/editor/editor_selection.hpp"
#include "hyperion/editor/editor_render_pipeline.hpp"

namespace Hyperion::Editor {

    class EditorApplication : public Application {
    public:
        EditorApplication(const ApplicationSettings &settings) : Application(settings) { }
    protected:
        void OnInit() override {
            EditorEngine::Init();
        }

        void OnUpdate(f32 delta_time) override {
            EditorEngine::Update(delta_time);
        }

        void OnTick() override {
            EditorEngine::Tick();
        }
    };

}

void Hyperion::RegisterApplicationTypes() {

}

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings::FromJsonFile("app.json");
    settings.render.path = Rendering::RenderPath::Custom;
    settings.render.custom_pipeline = new Editor::EditorRenderPipeline();

    return new Hyperion::Editor::EditorApplication(settings);
}