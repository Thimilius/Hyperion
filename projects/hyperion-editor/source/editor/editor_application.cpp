//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/assets/material.hpp>
#include <hyperion/assets/loader/font_loader.hpp>
#include <hyperion/assets/loader/image_loader.hpp>
#include <hyperion/assets/loader/mesh_loader.hpp>
#include <hyperion/assets/utilities/mesh_generator.hpp>
#include <hyperion/core/engine.hpp>
#include <hyperion/core/random.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/io/file_system.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/ecs/component/components/components.hpp>
#include <hyperion/ecs/component/components/utilities/camera_controller.hpp>
#include <hyperion/ecs/component/components/utilities/transform_utilities.hpp>
#include <hyperion/ecs/system/systems/render_systems.hpp>
#include <hyperion/ecs/system/systems/ui_systems.hpp>
#include <hyperion/ecs/world/world_manager.hpp>
#include <hyperion/ecs/world/world_serializer.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/render/pipelines/forward/forward_render_pipeline.hpp>
#include <hyperion/ui/ui_factory.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_render_pipeline.hpp"

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

    const uint32 UI_HEADER_SIZE = 25;
    const Color UI_NORMAL_COLOR = Color(0.25f, 0.25f, 0.25f, 1.0f);
    const Color UI_HIGHLIGHT_COLOR = Color(0.0f, 0.4f, 0.8f, 1.0f);

    World *g_world;
    EntityId g_camera;
    EntityId g_light;
    EntityId g_parent;
    EntityId g_child;

    UIViewComponent g_editor_ui_view;
    UIElement *g_root_element;
    UIElement *g_header_ui_element;
    UIElement *g_render_ui_element;
    UIButton *g_child_ui_element;
    UILabel *g_label_stats;
    UIToggle *g_vsync_toggle;
    UIToggle *g_grid_toggle;
    UIToggle *g_bounds_toggle;

    CameraController *g_camera_controller;

    //--------------------------------------------------------------
    void EditorApplication::OnInitialize() {
        RenderEngine::GetPipeline()->SetRenderTargetSize(Display::GetWidth(), Display::GetHeight() - UI_HEADER_SIZE);

        {
            g_world = WorldManager::CreateWorld();
            WorldManager::SetActiveWorld(g_world);

            g_camera = g_world->CreateEntity(EntityPrimitive::Camera);
            g_light = g_world->CreateEntity(EntityPrimitive::DirectionalLight);

            g_camera_controller = new LookAroundCameraController(g_camera);
            g_camera_controller->Reset(g_world);
            LocalTransformComponent *camera_transform = g_world->GetComponent<LocalTransformComponent>(g_camera);

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
        }

        {
            g_root_element = UIFactory::CreateRoot();

            g_render_ui_element = UIFactory::CreateElement();
            g_render_ui_element->SetAnchorPreset(UIAnchorPreset::StretchAll);
            g_render_ui_element->SetAnchorOffsetMax(Vector2(0.0f, UI_HEADER_SIZE));
            g_render_ui_element->GetRenderer().texture = RenderEngine::GetPipeline()->GetTargetRenderTexture();
            g_render_ui_element->GetRenderer().render_texture_attachment_index = 0;
            g_render_ui_element->GetRenderer().enable_blending = false;
            g_render_ui_element->GetHierarchy().SetParent(g_root_element);

            g_header_ui_element = UIFactory::CreateElement();
            g_header_ui_element->SetSize(Vector2(0.0f, UI_HEADER_SIZE - 1));
            g_header_ui_element->SetAnchorPreset(UIAnchorPreset::TopStretchHorizontal);
            g_header_ui_element->GetStyle().SetColor(UI_NORMAL_COLOR);
            g_header_ui_element->GetHierarchy().SetParent(g_root_element);

            UIElement *g_header_seperator_ui_element = UIFactory::CreateElement();
            g_header_seperator_ui_element->SetAnchorPreset(UIAnchorPreset::TopStretchHorizontal);
            g_header_seperator_ui_element->SetSize(Vector2(0.0f, 1.0f));
            g_header_seperator_ui_element->SetPosition(Vector2(0.0f, -static_cast<float32>(UI_HEADER_SIZE - 1)));
            g_header_seperator_ui_element->GetStyle().SetColor(UI_HIGHLIGHT_COLOR);
            g_header_seperator_ui_element->GetHierarchy().SetParent(g_root_element);

            Font *consola_font = FontLoader::LoadFont("data/fonts/consola.ttf", 12, FontCharacterSet::LatinSupplement);
            Font *icon_font = FontLoader::LoadFont("data/fonts/font_awesome_solid.otf", 12, FontCharacterSet::All);

            g_label_stats = UIFactory::CreateLabel();
            g_label_stats->SetFont(consola_font);
            g_label_stats->SetAlignment(UITextAlignment::MiddleRight);
            g_label_stats->SetAnchorPreset(UIAnchorPreset::StretchAll);
            g_label_stats->SetAnchorOffsetMax(Vector2(5.0f, 0.0f));
            g_label_stats->GetStyle().GetShadow().enabled = true;
            g_label_stats->GetHierarchy().SetParent(g_header_ui_element);

            auto create_toggle = [](UIElement *parent, float32 x_offset, Font *font, const String &text) -> UIToggle * {
                UIToggle *ui_toggle = UIFactory::CreateToggle();
                ui_toggle->SetSize(Vector2(25.0f, 0.0f));
                ui_toggle->SetAnchorPreset(UIAnchorPreset::LeftStretchVertical);
                ui_toggle->SetPosition(Vector2(x_offset, 0.0f));
                ui_toggle->GetStyle().SetColor(UI_NORMAL_COLOR);
                ui_toggle->SetToggleOnColor(UI_HIGHLIGHT_COLOR);
                ui_toggle->SetToggleOffColor(Color::White());
                ui_toggle->GetHierarchy().SetParent(parent);
                ui_toggle->GetHierarchy().RemoveChildren();
                UILabel *toggle_label = UIFactory::CreateLabel();
                toggle_label->GetHierarchy().SetParent(ui_toggle);
                toggle_label->SetAnchorPreset(UIAnchorPreset::StretchAll);
                toggle_label->SetFont(font);
                toggle_label->SetText(text);
                toggle_label->GetStyle().GetShadow().enabled = true;
                ui_toggle->SetToggleGraphic(toggle_label);
                return ui_toggle;
            };

            g_vsync_toggle = create_toggle(g_header_ui_element, 0.0f, icon_font, "\uf108");
            g_vsync_toggle->SetIsOn(Rendering::RenderEngine::GetVSyncMode() != Rendering::VSyncMode::DontSync);
            g_vsync_toggle->RegisterToggleCallback([](bool8 is_on) {
                Rendering::RenderEngine::SetVSyncMode(
                    Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync ?
                    Rendering::VSyncMode::EveryVBlank :
                    Rendering::VSyncMode::DontSync);
            });
            g_grid_toggle = create_toggle(g_header_ui_element, 25.0f, icon_font, "\uf850");
            g_grid_toggle->SetIsOn(Rendering::RenderGizmos::GetShouldDrawGrid());
            g_grid_toggle->RegisterToggleCallback([](bool8 is_on) {
                Rendering::RenderGizmos::SetShouldDrawGrid(is_on);
            });
            g_bounds_toggle = create_toggle(g_header_ui_element, 50.0f, icon_font, "\uf247");
            g_bounds_toggle->SetIsOn(Rendering::RenderGizmos::GetShouldDrawAllBounds());
            g_bounds_toggle->RegisterToggleCallback([](bool8 is_on) {
                Rendering::RenderGizmos::SetShouldDrawAllBounds(is_on);
            });
            UIButton *camera_reset_button = UIFactory::CreateButton();
            camera_reset_button->SetSize(Vector2(25.0f, 0.0f));
            camera_reset_button->SetAnchorPreset(UIAnchorPreset::LeftStretchVertical);
            camera_reset_button->SetPosition(Vector2(75.0f, 0.0f));
            camera_reset_button->GetStyle().SetColor(UI_NORMAL_COLOR);
            camera_reset_button->GetHierarchy().SetParent(g_header_ui_element);
            UILabel *camera_reset_label = camera_reset_button->Q<UILabel>("");
            camera_reset_label->SetFont(icon_font);
            camera_reset_label->SetText("\uf03d");
            camera_reset_label->GetStyle().GetShadow().enabled = true;
            camera_reset_button->RegisterClickCallback([]() {
                g_camera_controller->Reset(g_world);
            });

            g_editor_ui_view.scaling_mode = UIScalingMode::ConstantPixelSize;
            g_editor_ui_view.root_element = g_root_element;

            UpdateStats();
        }
    }

    //--------------------------------------------------------------
    void EditorApplication::OnUpdate(float32 delta_time) {
        if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
            Exit();
        }
        if (Input::IsKeyDown(KeyCode::F1)) {
            GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
        }
        if (Input::IsKeyDown(KeyCode::F2)) {
            g_vsync_toggle->Toggle();
        }
        if (Input::IsKeyDown(KeyCode::F3)) {
            g_grid_toggle->Toggle();
        }
        if (Input::IsKeyDown(KeyCode::F4)) {
            g_bounds_toggle->Toggle();
        }
        if (Input::IsKeyDown(KeyCode::F5)) {
            if (Engine::GetEngineMode() == EngineMode::Editor) {
                EnterRuntime();
            } else {
                ExitRuntime();
            }
        }

        g_camera_controller->Update(g_world, delta_time);
        if (Input::IsKeyDown(KeyCode::R)) {
            g_camera_controller->Reset(g_world);
        }

        UpdateStats();

        if (Engine::GetEngineMode() == EngineMode::EditorRuntimePlaying) {
            g_world->GetComponent<LocalTransformComponent>(g_parent)->rotation = Quaternion::FromEulerAngles(0.0f, Math::Sin(Time::GetTime()) * 45.0f, 0.0f);
        }

        uint32 render_target_height = Display::GetHeight() - UI_HEADER_SIZE;
        g_world->GetComponent<CameraComponent>(g_camera)->viewport_clipping.height = render_target_height / static_cast<float32>(Display::GetHeight());
        if (Display::HasChangedSize()) {
            RenderEngine::GetPipeline()->SetRenderTargetSize(Display::GetWidth(), render_target_height);
        }

        {
            UIRebuildSystem::Run(&g_editor_ui_view);
            UIEventSystem::Run(&g_editor_ui_view);

            RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();
            Delegate<RenderFrameContextObjectUI &()> ui_object_adder;
            ui_object_adder.Connect<&RenderFrameContext::AddEditorUIObject>(&render_frame_context);
            UIRenderSystem::Run(&g_editor_ui_view, ui_object_adder);
        }
    }

    //--------------------------------------------------------------
    void EditorApplication::OnTick() {

    }

    //--------------------------------------------------------------
    void EditorApplication::UpdateStats() {
        String stats_format = "FPS: {} ({:.2f}ms) - Draw calls: {}, Vertices: {}, Triangles: {}";
        RenderStats render_stats = Rendering::RenderEngine::GetStats();
        String stats_title = StringUtils::Format(stats_format, Time::GetFPS(), Time::GetFrameTime(), render_stats.draw_calls, render_stats.vertex_count, render_stats.triangle_count);
        g_label_stats->SetText(stats_title);
    }

}

//--------------------------------------------------------------
Hyperion::Application *Hyperion::CreateApplication() {
    return new Hyperion::Editor::EditorApplication();
}