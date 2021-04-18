//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/core/app/application.hpp>
#include <hyperion/entity/world_manager.hpp>

//------------------------- Namespaces -------------------------
using namespace Hyperion;

//-------------------- Definition Namespace --------------------
namespace Sandbox {

    class SandboxApplication : public Application {
    public:
        SandboxApplication(const ApplicationSettings &settings) : Application(settings) { }
    protected:
        void OnInitialize() override {
            WorldManager::SetActiveWorld(WorldManager::CreateWorld());
        }

        void OnUpdate(float32 delta_time) override {
            if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
                Exit();
            }
            if (Input::IsKeyDown(KeyCode::F1)) {
                GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
            }
        }

        void OnShutdown() override {

        }
    };

}

//--------------------------------------------------------------
Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings();
    settings.render.graphics_backend = Graphics::GraphicsBackend::OpenGL;
    return new Sandbox::SandboxApplication(settings);
}