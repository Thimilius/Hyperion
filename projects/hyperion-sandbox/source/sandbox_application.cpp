//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/entity/world_manager.hpp>

#include "sandbox/sandbox_application.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion;

//-------------------- Definition Namespace --------------------
namespace Sandbox {

    //--------------------------------------------------------------
    SandboxApplication::SandboxApplication(const Hyperion::ApplicationSettings &settings) : Application(settings) { }

    //--------------------------------------------------------------
    void SandboxApplication::OnInitialize() {
        WorldManager::SetActiveWorld(WorldManager::CreateWorld());

        UpdateTitle();
    }

    //--------------------------------------------------------------
    void SandboxApplication::OnUpdate(float32 delta_time) {
        if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
            Exit();
        }
        if (Input::IsKeyDown(KeyCode::F1)) {
            GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
        }

        UpdateTitle();
    }

    //--------------------------------------------------------------
    void SandboxApplication::UpdateTitle() {
        String title = StringUtils::Format("Hyperion - FPS: {} ({:.2f}ms)", Time::GetFPS(), Time::GetFrameTime());
        GetWindow()->SetTitle(title);
    }

}

//--------------------------------------------------------------
Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings();
    settings.render.graphics_backend = Graphics::GraphicsBackend::Vulkan;
    return new Sandbox::SandboxApplication(settings);
}