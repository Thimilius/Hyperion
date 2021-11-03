//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_ui.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/assets/loader/font_loader.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/ecs/system/systems/render_systems.hpp>
#include <hyperion/ecs/system/systems/ui_systems.hpp>
#include <hyperion/render/render_engine.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_style.hpp"
#include "hyperion/editor/editor_camera.hpp"

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

        s_render_ui_element = UIFactory::CreateElement();
        s_render_ui_element->SetAnchorPreset(UIAnchorPreset::StretchAll);
        s_render_ui_element->SetAnchorOffsetMax(Vector2(0.0f, EditorStyle::HEADER_SIZE));
        s_render_ui_element->GetRenderer().texture = RenderEngine::GetPipeline()->GetTargetRenderTexture();
        s_render_ui_element->GetRenderer().render_texture_attachment_index = 0;
        s_render_ui_element->GetRenderer().enable_blending = false;
        s_render_ui_element->GetHierarchy().SetParent(s_root_element);

        s_header_ui_element = UIFactory::CreateElement();
        s_header_ui_element->SetSize(Vector2(0.0f, EditorStyle::HEADER_SIZE - 1));
        s_header_ui_element->SetAnchorPreset(UIAnchorPreset::TopStretchHorizontal);
        s_header_ui_element->GetStyle().SetColor(EditorStyle::NORMAL);
        s_header_ui_element->GetHierarchy().SetParent(s_root_element);

        UIElement *header_seperator_ui_element = UIFactory::CreateElement();
        header_seperator_ui_element->SetAnchorPreset(UIAnchorPreset::TopStretchHorizontal);
        header_seperator_ui_element->SetSize(Vector2(0.0f, 1.0f));
        header_seperator_ui_element->SetPosition(Vector2(0.0f, -static_cast<float32>(EditorStyle::HEADER_SIZE - 1)));
        header_seperator_ui_element->GetStyle().SetColor(EditorStyle::HIGHLIGHT);
        header_seperator_ui_element->GetHierarchy().SetParent(s_root_element);

        s_label_stats = UIFactory::CreateLabel();
        s_label_stats->SetFont(s_font_text);
        s_label_stats->SetAlignment(UITextAlignment::MiddleRight);
        s_label_stats->SetAnchorPreset(UIAnchorPreset::StretchAll);
        s_label_stats->SetAnchorOffsetMax(Vector2(5.0f, 0.0f));
        s_label_stats->GetStyle().GetShadow().enabled = true;
        s_label_stats->GetHierarchy().SetParent(s_header_ui_element);

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

        s_toggle_vsync = create_toggle(s_header_ui_element, 0.0f, s_font_icon, "\uf108");
        s_toggle_vsync->SetIsOn(Rendering::RenderEngine::GetVSyncMode() != Rendering::VSyncMode::DontSync);
        s_toggle_vsync->RegisterToggleCallback([](bool8 is_on) {
            Rendering::RenderEngine::SetVSyncMode(
                Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync ?
                Rendering::VSyncMode::EveryVBlank :
                Rendering::VSyncMode::DontSync);
        });
        s_toggle_grid = create_toggle(s_header_ui_element, 25.0f, s_font_icon, "\uf850");
        s_toggle_grid->SetIsOn(Rendering::RenderGizmos::GetShouldDrawGrid());
        s_toggle_grid->RegisterToggleCallback([](bool8 is_on) {
            Rendering::RenderGizmos::SetShouldDrawGrid(is_on);
        });
        s_toggle_grounds = create_toggle(s_header_ui_element, 50.0f, s_font_icon, "\uf247");
        s_toggle_grounds->SetIsOn(Rendering::RenderGizmos::GetShouldDrawAllBounds());
        s_toggle_grounds->RegisterToggleCallback([](bool8 is_on) {
            Rendering::RenderGizmos::SetShouldDrawAllBounds(is_on);
        });
        UIButton *camera_reset_button = UIFactory::CreateButton();
        camera_reset_button->SetSize(Vector2(25.0f, 0.0f));
        camera_reset_button->SetAnchorPreset(UIAnchorPreset::LeftStretchVertical);
        camera_reset_button->SetPosition(Vector2(75.0f, 0.0f));
        camera_reset_button->GetStyle().SetColor(EditorStyle::NORMAL);
        camera_reset_button->GetHierarchy().SetParent(s_header_ui_element);
        UILabel *camera_reset_label = camera_reset_button->Q<UILabel>("");
        camera_reset_label->SetFont(s_font_icon);
        camera_reset_label->SetText("\uf03d");
        camera_reset_label->GetStyle().GetShadow().enabled = true;
        camera_reset_button->RegisterClickCallback([]() {
            EditorCamera::Reset();
        });

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
    void EditorUI::UpdateStats() {
        String stats_format = "FPS: {} ({:.2f}ms) - Draw calls: {}, Vertices: {}, Triangles: {}";
        RenderStats render_stats = Rendering::RenderEngine::GetStats();
        String stats_title = StringUtils::Format(stats_format, Time::GetFPS(), Time::GetFrameTime(), render_stats.draw_calls, render_stats.vertex_count, render_stats.triangle_count);
        s_label_stats->SetText(stats_title);
    }

}