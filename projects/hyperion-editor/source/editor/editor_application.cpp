#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>

#include "hyperion/editor/editor_camera_controller.hpp"

namespace Hyperion::Editor {

    class EditorApplication : public Application {
    public:
        EditorApplication(const ApplicationSettings &settings) : Application(settings) { }
    protected:
        void OnInit() override {
            m_world = WorldManager::CreateWorld();
            WorldManager::SetActiveWorld(m_world);

            m_camera_entity = Entity::Create("Camera");
            m_camera_entity->AddComponent<Camera>();
            m_camera_entity->AddComponent<EditorCameraController>();
        }

        void OnUpdate(float32 delta_time) override {
            if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
                Exit();
            }
            if (Input::IsKeyDown(KeyCode::F1)) {
                GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
            }

            for (size_t i = 0; i < 1000; i++) {
                Entity *e = Entity::Create("Entity");
                Object::Destroy(e);
            }
        }

        void OnTick() override {
            float32 memory = static_cast<float32>(MemoryStats::GetGlobalMemory()) / 1000.0f / 1000.0f;
            String title = StringUtils::Format("Hyperion - FPS: {} ({:.2f}ms) - Memory: {:.2f}MB", Time::GetFPS(), Time::GetFrameTime(), memory);
            GetWindow()->SetTitle(title);
        }
    private:
        World *m_world;
        Entity *m_camera_entity;
        Camera *m_camera;
    };

}

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings::FromJsonFile("app.json");
    settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
    return new Hyperion::Editor::EditorApplication(settings);
}