//--------------------- Definition Include ---------------------
#include "sandbox/sandbox_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/ecs/component/components.hpp>
#include <hyperion/ecs/system/transform_system.hpp>
#include <hyperion/ecs/world/world_manager.hpp>

//------------------------- Namespaces -------------------------
using namespace Hyperion;

//-------------------- Definition Namespace --------------------
namespace Sandbox {

    //--------------------------------------------------------------
    void SandboxApplication::OnSetup(ApplicationSettings &settings) {
        settings.render.graphics_backend = Graphics::GraphicsBackend::OpenGL;
        settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
    }

    //--------------------------------------------------------------
    void SandboxApplication::OnInitialize() {
        UpdateTitle();

        World *world = WorldManager::CreateWorld();
        WorldManager::SetActiveWorld(world);
        world->CreateEntity(EntityPrimitive::Camera);
        for (size_t i = 0; i < 1024; i++) {
            EntityId entity = world->CreateEntity();
        }
    }

    //--------------------------------------------------------------
    void SandboxApplication::OnUpdate(float32 delta_time) {
        if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
            Exit();
        }
        if (Input::IsKeyDown(KeyCode::F1)) {
            GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
        }
    }

    //--------------------------------------------------------------
    void SandboxApplication::OnTick() {
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
    return new Sandbox::SandboxApplication();
}