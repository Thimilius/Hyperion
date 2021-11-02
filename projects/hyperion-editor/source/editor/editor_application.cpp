//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/assets/material.hpp>
#include <hyperion/assets/loader/image_loader.hpp>
#include <hyperion/core/engine.hpp>
#include <hyperion/core/random.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/ecs/component/components/components.hpp>
#include <hyperion/ecs/component/components/utilities/camera_controller.hpp>
#include <hyperion/ecs/component/components/utilities/transform_utilities.hpp>
#include <hyperion/ecs/world/world_manager.hpp>
#include <hyperion/ecs/world/world_serializer.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/ui/ui_factory.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_render_pipeline.hpp"
#include "hyperion/editor/editor_style.hpp"
#include "hyperion/editor/editor_ui.hpp"
#include "hyperion/editor/editor_world_camera.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;
using namespace Hyperion::UI;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorApplication::EnterRuntime() {
        Engine::SetEngineMode(EngineMode::EditorRuntimePlaying);
    }

    //--------------------------------------------------------------
    void EditorApplication::PauseRuntime() {
        Engine::SetEngineMode(EngineMode::EditorRuntimePaused);
    }

    //--------------------------------------------------------------
    void EditorApplication::ExitRuntime() {
        Engine::SetEngineMode(EngineMode::Editor);
    }

    //--------------------------------------------------------------
    void EditorApplication::OnSetup(ApplicationSettings &settings) {
        settings.render.backend = Rendering::RenderBackend::OpenGL;
        settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
        settings.render.vsync_mode = Rendering::VSyncMode::DontSync;
        settings.render.pipeline = Rendering::RenderPipeline::Custom;
        settings.render.custom_pipeline = new EditorRenderPipeline();
    }

    World *g_world;
    EntityId g_camera;
    EntityId g_light;
    EntityId g_parent;
    EntityId g_child;

    //--------------------------------------------------------------
    void EditorApplication::OnInitialize() {
        EditorUI::Initialize();

        {
            g_world = WorldManager::CreateWorld();
            WorldManager::SetActiveWorld(g_world);

            g_camera = g_world->CreateEntity(EntityPrimitive::Camera);
            g_light = g_world->CreateEntity(EntityPrimitive::DirectionalLight);

            g_parent = g_world->CreateEntity(EntityPrimitive::Sphere);

            g_child = g_world->CreateEntity(EntityPrimitive::Cube);
            g_world->GetComponent<LocalTransformComponent>(g_child)->position = Vector3(2.0f, 0.0f, 0.0f);
            g_world->GetHierarchy()->SetParent(g_child, g_parent);

            std::unique_ptr<Image> image;
            image.reset(ImageLoader::Load("icon/icon.png").Unwrap());
            Texture2DParameters parameters;
            parameters.format = TextureFormat::RGBA32;
            parameters.width = image->GetWidth();
            parameters.height = image->GetHeight();
            parameters.attributes.anisotropic_filter = TextureAnisotropicFilter::Times16;
            Texture2D *texture = AssetManager::CreateTexture2D(parameters, image->GetPixels());
            Material *material = AssetManager::CreateMaterial(AssetManager::GetShaderPrimitive(ShaderPrimitive::Unlit));
            material->SetTexture("m_texture", texture);
            EntityId quad = g_world->CreateEntity(EntityPrimitive::Quad);
            g_world->GetComponent<MeshComponent>(quad)->material = material;
            g_world->GetComponent<LocalTransformComponent>(quad)->position = Vector3(0.0f, 2.0f, 0.0f);

            UIElement *root_element = UIFactory::CreateRoot();

            EntityId ui = g_world->CreateEntity();
            UIViewComponent *ui_view = g_world->AddComponent<UIViewComponent>(ui);
            ui_view->root_element = root_element;

            EditorWorldCamera::Initialize(g_camera, g_world);
        }
    }

    //--------------------------------------------------------------
    void EditorApplication::OnUpdate(float32 delta_time) {
        EditorUI::Update();
        EditorWorldCamera::Update(delta_time, g_camera, g_world);

        if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
            Exit();
        }
        if (Input::IsKeyDown(KeyCode::F1)) {
            GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
        }
        if (Input::IsKeyDown(KeyCode::F5)) {
            if (Engine::GetEngineMode() == EngineMode::Editor) {
                EnterRuntime();
            } else {
                ExitRuntime();
            }
        }

        if (Engine::GetEngineMode() == EngineMode::EditorRuntimePlaying) {
            g_world->GetComponent<LocalTransformComponent>(g_parent)->rotation = Quaternion::FromEulerAngles(0.0f, Math::Sin(Time::GetTime()) * 45.0f, 0.0f);
        }
    }

    //--------------------------------------------------------------
    void EditorApplication::OnTick() {

    }

}

//--------------------------------------------------------------
Hyperion::Application *Hyperion::CreateApplication() {
    return new Hyperion::Editor::EditorApplication();
}