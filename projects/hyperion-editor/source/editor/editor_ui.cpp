//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_ui.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/assets/loader/font_loader.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/ecs/system/systems/render_systems.hpp>
#include <hyperion/ecs/system/systems/ui_systems.hpp>
#include <hyperion/render/render_engine.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_application.hpp"
#include "hyperion/editor/editor_camera.hpp"
#include "hyperion/editor/editor_style.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;
using namespace Hyperion::UI;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorUI::Initialize() {
        s_font_text = FontLoader::LoadFont("data/fonts/consola.ttf", EditorStyle::FONT_SIZE, FontCharacterSet::LatinSupplement);
        s_font_icon = FontLoader::LoadFont("data/fonts/font_awesome_solid.otf", EditorStyle::FONT_SIZE, FontCharacterSet::All);

        s_root_element = UIFactory::CreateRoot();
        s_ui_view.root_element = s_root_element;
        s_ui_view.scaling_mode = UIScalingMode::ConstantPixelSize;

        // Preview
        {
            UIElement *preview = UIFactory::CreateElement();
            preview->SetAnchorPreset(UIAnchorPreset::StretchAll);
            preview->SetAnchorOffsetMax(Vector2(0.0f, EditorStyle::HEADER_SIZE));
            preview->SetAnchorOffsetMin(Vector2(EditorStyle::SIDEBAR_SIZE, EditorStyle::FOOTER_SIZE));
            preview->GetStyle().SetColor(EditorStyle::HIGHLIGHT);
            preview->GetHierarchy().SetParent(s_root_element);

            UIElement *preview_header = UIFactory::CreateElement();
            preview_header->SetSize(Vector2(0.0f, EditorStyle::HEADER_SIZE));
            preview_header->SetAnchorPreset(UIAnchorPreset::TopStretchHorizontal);
            preview_header->GetStyle().SetColor(EditorStyle::NORMAL);
            preview_header->GetHierarchy().SetParent(preview);

            s_preview_container_ui_element = UIFactory::CreateElement();
            s_preview_container_ui_element->SetAnchorPreset(UIAnchorPreset::StretchAll);
            s_preview_container_ui_element->SetAnchorOffsetMax(Vector2(0.0f, EditorStyle::HEADER_SIZE));
            s_preview_container_ui_element->GetStyle().SetColor(EditorStyle::HIGHLIGHT);
            s_preview_container_ui_element->GetHierarchy().SetParent(preview);

            s_preview_runtime_ui_element = UIFactory::CreateElement();
            s_preview_runtime_ui_element->SetAnchorPreset(UIAnchorPreset::StretchAll);
            s_preview_runtime_ui_element->GetRenderer().texture = EditorApplication::GetRenderPipeline()->GetTargetRenderTexture();
            s_preview_runtime_ui_element->GetRenderer().render_texture_attachment_index = 0;
            s_preview_runtime_ui_element->GetRenderer().enable_blending = false;
            s_preview_runtime_ui_element->GetHierarchy().SetParent(s_preview_container_ui_element);

            s_preview_editor_ui_element = UIFactory::CreateElement();
            s_preview_editor_ui_element->SetAnchorPreset(UIAnchorPreset::StretchAll);
            s_preview_editor_ui_element->GetRenderer().texture = EditorApplication::GetRenderPipeline()->GetEditorTargetRenderTexture();
            s_preview_editor_ui_element->GetRenderer().render_texture_attachment_index = 0;
            s_preview_editor_ui_element->GetRenderer().enable_blending = false;
            s_preview_editor_ui_element->GetHierarchy().SetParent(s_preview_container_ui_element);

            auto create_toggle = [](UIElement *parent, float32 x_offset, Font *font, const String &text) -> UIToggle * {
                UIToggle *ui_toggle = UIFactory::CreateToggle();
                ui_toggle->SetSize(Vector2(25.0f, 0.0f));
                ui_toggle->SetAnchorPreset(UIAnchorPreset::LeftStretchVertical);
                ui_toggle->SetPosition(Vector2(x_offset, 0.0f));
                ui_toggle->GetStyle().SetColor(EditorStyle::NORMAL);
                ui_toggle->SetToggleOnColor(EditorStyle::HIGHLIGHT);
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

            s_toggle_vsync = create_toggle(preview_header, 0.0f, s_font_icon, "\uf108");
            s_toggle_vsync->SetIsOn(Rendering::RenderEngine::GetVSyncMode() != Rendering::VSyncMode::DontSync);
            s_toggle_vsync->RegisterToggleCallback([](bool8 is_on) {
                Rendering::RenderEngine::SetVSyncMode(
                    Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync ?
                    Rendering::VSyncMode::EveryVBlank :
                    Rendering::VSyncMode::DontSync);
            });
            s_toggle_grid = create_toggle(preview_header, 25.0f, s_font_icon, "\uf850");
            s_toggle_grid->SetIsOn(Rendering::RenderGizmos::GetShouldDrawGrid());
            s_toggle_grid->RegisterToggleCallback([](bool8 is_on) {
                Rendering::RenderGizmos::SetShouldDrawGrid(is_on);
            });
            s_toggle_grounds = create_toggle(preview_header, 50.0f, s_font_icon, "\uf247");
            s_toggle_grounds->SetIsOn(Rendering::RenderGizmos::GetShouldDrawAllBounds());
            s_toggle_grounds->RegisterToggleCallback([](bool8 is_on) {
                Rendering::RenderGizmos::SetShouldDrawAllBounds(is_on);
            });
            UIButton *camera_reset_button = UIFactory::CreateButton();
            camera_reset_button->SetSize(Vector2(25.0f, 0.0f));
            camera_reset_button->SetAnchorPreset(UIAnchorPreset::LeftStretchVertical);
            camera_reset_button->SetPosition(Vector2(75.0f, 0.0f));
            camera_reset_button->GetStyle().SetColor(EditorStyle::NORMAL);
            camera_reset_button->GetHierarchy().SetParent(preview_header);
            UILabel *camera_reset_label = camera_reset_button->Q<UILabel>("");
            camera_reset_label->SetFont(s_font_icon);
            camera_reset_label->SetText("\uf03d");
            camera_reset_label->GetStyle().GetShadow().enabled = true;
            camera_reset_button->RegisterClickCallback([]() {
                EditorCamera::Reset();
            });

            s_label_stats = UIFactory::CreateLabel();
            s_label_stats->SetFont(s_font_text);
            s_label_stats->SetAlignment(UITextAlignment::MiddleRight);
            s_label_stats->SetAnchorPreset(UIAnchorPreset::StretchAll);
            s_label_stats->SetAnchorOffsetMax(Vector2(5.0f, 0.0f));
            s_label_stats->GetStyle().GetShadow().enabled = true;
            s_label_stats->GetHierarchy().SetParent(preview_header);
        }
        
        // Header.
        {
            UIElement *top_bar = UIFactory::CreateElement();
            top_bar->SetSize(Vector2(0.0f, EditorStyle::HEADER_SIZE - 1));
            top_bar->SetAnchorPreset(UIAnchorPreset::TopStretchHorizontal);
            top_bar->GetStyle().SetColor(EditorStyle::NORMAL);
            top_bar->GetHierarchy().SetParent(s_root_element);

            UIElement *top_bar_container = UIFactory::CreateElement();
            top_bar_container->SetAnchorPreset(UIAnchorPreset::TopStretchHorizontal);
            top_bar_container->SetSize(Vector2(0.0f, 1.0f));
            top_bar_container->SetPosition(Vector2(0.0f, -static_cast<float32>(EditorStyle::HEADER_SIZE - 1)));
            top_bar_container->GetStyle().SetColor(EditorStyle::HIGHLIGHT);
            top_bar_container->GetHierarchy().SetParent(s_root_element);
        }

        // Sidebar.
        {
            UIElement *left_bar = UIFactory::CreateElement();
            left_bar->SetSize(Vector2(EditorStyle::SIDEBAR_SIZE, 0.0f));
            left_bar->SetAnchorPreset(UIAnchorPreset::LeftStretchVertical);
            left_bar->SetAnchorOffsetMax(Vector2(0.0f, EditorStyle::HEADER_SIZE));
            left_bar->SetAnchorOffsetMin(Vector2(0.0f, EditorStyle::FOOTER_SIZE));
            left_bar->GetStyle().SetColor(EditorStyle::NORMAL_DARK);
            left_bar->GetHierarchy().SetParent(s_root_element);

            UIElement *left_bar_container = UIFactory::CreateElement();
            left_bar_container->SetAnchorPreset(UIAnchorPreset::StretchAll);
            left_bar_container->SetAnchorOffsetMax(Vector2(3.0f, 3.0f));
            left_bar_container->SetAnchorOffsetMin(Vector2(3.0f, 3.0f));
            left_bar_container->GetStyle().SetColor(EditorStyle::NORMAL);
            left_bar_container->GetHierarchy().SetParent(left_bar);
        }
        
        // Footer.
        {
            UIElement *bottom_bar = UIFactory::CreateElement();
            bottom_bar->SetSize(Vector2(0.0f, EditorStyle::FOOTER_SIZE));
            bottom_bar->SetAnchorPreset(UIAnchorPreset::BottomStretchHorizontal);
            bottom_bar->GetStyle().SetColor(EditorStyle::NORMAL_DARK);
            bottom_bar->GetHierarchy().SetParent(s_root_element);

            UIElement *bottom_bar_container = UIFactory::CreateElement();
            bottom_bar_container->SetAnchorPreset(UIAnchorPreset::StretchAll);
            bottom_bar_container->SetAnchorOffsetMax(Vector2(3.0f, 3.0f));
            bottom_bar_container->SetAnchorOffsetMin(Vector2(3.0f, 3.0f));
            bottom_bar_container->GetStyle().SetColor(EditorStyle::NORMAL);
            bottom_bar_container->GetHierarchy().SetParent(bottom_bar);
        }

        UpdateStats();
    }

    //--------------------------------------------------------------
    void EditorUI::Update() {
        UpdateStats();

        if (Input::IsKeyDown(KeyCode::F2)) {
            s_toggle_vsync->Toggle();
        }
        if (Input::IsKeyDown(KeyCode::F3)) {
            s_toggle_grid->Toggle();
        }
        if (Input::IsKeyDown(KeyCode::F4)) {
            s_toggle_grounds->Toggle();
        }

        if (Input::IsKeyDown(KeyCode::F6)) {
            if (s_preview_editor_ui_element->GetStyle().GetVisibility() == UIVisibility::Visible) {
                s_preview_editor_ui_element->GetStyle().SetVisibility(UIVisibility::Hidden);
                s_preview_runtime_ui_element->GetStyle().SetVisibility(UIVisibility::Visible);
            } else {
                s_preview_editor_ui_element->GetStyle().SetVisibility(UIVisibility::Visible);
                s_preview_runtime_ui_element->GetStyle().SetVisibility(UIVisibility::Hidden);
            }
        }

        {
            UIRebuildSystem::Run(&s_ui_view);
            UIEventSystem::Run(&s_ui_view);
            RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();
            Delegate<RenderFrameContextObjectUI &()> ui_object_adder;
            ui_object_adder.Connect<&RenderFrameContext::AddEditorUIObject>(&render_frame_context);
            UIRenderSystem::Run(&s_ui_view, ui_object_adder);
        }
    }

    //--------------------------------------------------------------
    RectInt EditorUI::GetPreviewRect() {
        if (s_preview_container_ui_element == nullptr) {
            return { 0, 0, 1280, 720 };
        } else {
            Rect rect = s_preview_container_ui_element->GetWorldRect();
            return { static_cast<int32>(rect.x), static_cast<int32>(rect.y), static_cast<int32>(rect.width), static_cast<int32>(rect.height) };
        }
    }

    //--------------------------------------------------------------
    void EditorUI::UpdateStats() {
        String stats_format = "FPS: {} ({:.2f}ms) - Draw calls: {}, Vertices: {}, Triangles: {}";
        RenderStats render_stats = Rendering::RenderEngine::GetStats();
        String stats_title = StringUtils::Format(stats_format, Time::GetFPS(), Time::GetFrameTime(), render_stats.draw_calls, render_stats.vertex_count, render_stats.triangle_count);
        s_label_stats->SetText(stats_title);
    }

}