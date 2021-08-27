//--------------------- Definition Include ---------------------
#include "sandbox/sandbox_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/core/app/time.hpp>

#include <hyperion/entity/world.hpp>

//------------------------- Namespaces -------------------------
using namespace Hyperion;

//-------------------- Definition Namespace --------------------
namespace Sandbox {

    //--------------------------------------------------------------
    void SandboxApplication::OnSetup(ApplicationSettings &settings) {
        settings.render.graphics_backend = Graphics::GraphicsBackend::OpenGL;
    }

    World world;
    struct TransformComponent {
        float32 position;
    };
    struct TagComponent {
        String tag;
    };

    //--------------------------------------------------------------
    void SandboxApplication::OnInitialize() {
        UpdateTitle();

        world.RegisterComponent<TransformComponent>();
        world.RegisterComponent<TagComponent>();

        for (size_t i = 0; i < 1000; i++) {
            auto entity = world.CreateEntity();
            world.AddComponent<TransformComponent>(entity);
            world.AddComponent<TagComponent>(entity);
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

        auto view = world.GetView<TransformComponent, TagComponent>();
        for (EntityId id : view) {
            TransformComponent *transform = world.GetComponent<TransformComponent>(id);
            TagComponent *tag = world.GetComponent<TagComponent>(id);
            transform->position += 1.0f;
            tag->tag = "Hello there";
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