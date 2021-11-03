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
#include "hyperion/editor/editor_camera.hpp"
#include "hyperion/editor/editor_render_pipeline.hpp"
#include "hyperion/editor/editor_style.hpp"
#include "hyperion/editor/editor_ui.hpp"

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
        s_render_pipeline = new EditorRenderPipeline();

        settings.render.backend = Rendering::RenderBackend::OpenGL;
        settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
        settings.render.vsync_mode = Rendering::VSyncMode::DontSync;
        settings.render.pipeline = Rendering::RenderPipeline::Custom;
        settings.render.custom_pipeline = s_render_pipeline;
    }

    EntityId g_camera;
    EntityId g_light;
    EntityId g_parent;
    EntityId g_child;

    //--------------------------------------------------------------
    void EditorApplication::OnInitialize() {
        EditorUI::Initialize();

        {
            s_world = WorldManager::CreateWorld();
            WorldManager::SetActiveWorld(s_world);

            g_camera = s_world->CreateEntity(EntityPrimitive::Camera);
            g_light = s_world->CreateEntity(EntityPrimitive::DirectionalLight);

            g_parent = s_world->CreateEntity(EntityPrimitive::Sphere);

            g_child = s_world->CreateEntity(EntityPrimitive::Cube);
            s_world->GetComponent<LocalTransformComponent>(g_child)->position = Vector3(2.0f, 0.0f, 0.0f);
            s_world->GetHierarchy()->SetParent(g_child, g_parent);

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
            EntityId quad = s_world->CreateEntity(EntityPrimitive::Quad);
            s_world->GetComponent<MeshComponent>(quad)->material = material;
            s_world->GetComponent<LocalTransformComponent>(quad)->position = Vector3(0.0f, 2.0f, 0.0f);

            UIElement *root_element = UIFactory::CreateRoot();

            EntityId ui = s_world->CreateEntity();
            UIViewComponent *ui_view = s_world->AddComponent<UIViewComponent>(ui);
            ui_view->root_element = root_element;

            EditorCamera::Initialize();
        }
    }

    //--------------------------------------------------------------
    void EditorApplication::OnUpdate(float32 delta_time) {
        EditorUI::Update();
        EditorCamera::Update(delta_time);

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
            s_world->GetComponent<LocalTransformComponent>(g_parent)->rotation = Quaternion::FromEulerAngles(0.0f, Math::Sin(Time::GetTime()) * 45.0f, 0.0f);
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