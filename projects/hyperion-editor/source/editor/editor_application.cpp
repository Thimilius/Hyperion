﻿#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>

#include "hyperion/editor/editor_camera_controller.hpp"
#include "hyperion/editor/editor_render_pipeline.hpp"
#include "hyperion/editor/editor_world_view.hpp"

namespace Hyperion::Editor {

    class EditorApplication : public Application {
    public:
        EditorApplication(const ApplicationSettings &settings) : Application(settings) { }
    protected:
        void OnInitialize() override {
            World *world = WorldManager::CreateWorld();
            WorldManager::SetActiveWorld(world);

            Entity *entity = Entity::Create("Camera");
            entity->AddComponent<Camera>();
            entity->AddComponent<EditorCameraController>();
        }

        void OnUpdate(float32 delta_time) override {
            if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
                Exit();
            }
            if (Input::IsKeyDown(KeyCode::F1)) {
                GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
            }

            EditorWorldView::OnUpdate(delta_time);
        }

        void OnTick() override {
            float32 memory = static_cast<float32>(MemoryStats::GetGlobalMemory()) / 1000.0f / 1000.0f;
            String title = StringUtils::Format("Hyperion - FPS: {} ({:.2f}ms) - Memory: {:.2f}MB", Time::GetFPS(), Time::GetFrameTime(), memory);
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