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
#include "hyperion/editor/editor_selection.hpp"
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
        s_ui_view.scaling_mode = ScalingMode::ConstantPixelSize;

        // Header.
        {
            UIElement *top_bar = UIFactory::CreateElement();
            top_bar->SetSize(Vector2(0.0f, EditorStyle::HEADER_SIZE - 1));
            top_bar->SetAnchorPreset(AnchorPreset::TopStretchHorizontal);
            top_bar->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
            top_bar->GetHierarchy().SetParent(s_root_element);

            UIElement *top_bar_container = UIFactory::CreateElement();
            top_bar_container->SetAnchorPreset(AnchorPreset::TopStretchHorizontal);
            top_bar_container->SetSize(Vector2(0.0f, 1.0f));
            top_bar_container->SetPosition(Vector2(0.0f, -static_cast<float32>(EditorStyle::HEADER_SIZE - 1)));
            top_bar_container->GetStyle().SetColor(EditorStyle::COLOR_HIGHLIGHT);
            top_bar_container->GetHierarchy().SetParent(s_root_element);
        }

        // Sidebar.
        {
            UIElement *left_bar = UIFactory::CreateElement();
            left_bar->SetSize(Vector2(EditorStyle::SIDEBAR_SIZE, 0.0f));
            left_bar->SetAnchorPreset(AnchorPreset::LeftStretchVertical);
            left_bar->SetAnchorOffsetMax(Vector2(0.0f, EditorStyle::HEADER_SIZE));
            left_bar->SetAnchorOffsetMin(Vector2(0.0f, EditorStyle::FOOTER_SIZE));
            left_bar->GetStyle().SetColor(EditorStyle::COLOR_NORMAL_DARK);
            left_bar->GetHierarchy().SetParent(s_root_element);

            UIElement *left_bar_container = UIFactory::CreateElement();
            left_bar_container->SetAnchorPreset(AnchorPreset::StretchAll);
            left_bar_container->SetAnchorOffsetMax(Vector2(3.0f, 3.0f));
            left_bar_container->SetAnchorOffsetMin(Vector2(3.0f, 3.0f));
            left_bar_container->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
            left_bar_container->GetHierarchy().SetParent(left_bar);

            s_label_selection = UIFactory::CreateLabel();
            s_label_selection->SetFont(s_font_text);
            s_label_selection->SetAlignment(TextAlignment::TopCenter);
            s_label_selection->SetAnchorPreset(AnchorPreset::StretchAll);
            s_label_selection->SetAnchorOffsetMax(Vector2(0.0f, 4.0f));
            s_label_selection->GetStyle().GetShadow().enabled = true;
            s_label_selection->GetHierarchy().SetParent(left_bar_container);
        }

        // Footer.
        {
            UIElement *bottom_bar = UIFactory::CreateElement();
            bottom_bar->SetSize(Vector2(0.0f, EditorStyle::FOOTER_SIZE));
            bottom_bar->SetAnchorPreset(AnchorPreset::BottomStretchHorizontal);
            bottom_bar->GetStyle().SetColor(EditorStyle::COLOR_NORMAL_DARK);
            bottom_bar->GetHierarchy().SetParent(s_root_element);

            UIElement *bottom_bar_container = UIFactory::CreateElement();
            bottom_bar_container->SetAnchorPreset(AnchorPreset::StretchAll);
            bottom_bar_container->SetAnchorOffsetMax(Vector2(3.0f, 3.0f));
            bottom_bar_container->SetAnchorOffsetMin(Vector2(3.0f, 3.0f));
            bottom_bar_container->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
            bottom_bar_container->GetHierarchy().SetParent(bottom_bar);
        }

        // Preview
        {
            UIElement *preview = UIFactory::CreateElement();
            preview->SetAnchorPreset(AnchorPreset::StretchAll);
            preview->SetAnchorOffsetMax(Vector2(0.0f, EditorStyle::HEADER_SIZE));
            preview->SetAnchorOffsetMin(Vector2(EditorStyle::SIDEBAR_SIZE, EditorStyle::FOOTER_SIZE));
            preview->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
            preview->GetHierarchy().SetParent(s_root_element);

            UIElement *preview_header = UIFactory::CreateElement();
            preview_header->SetSize(Vector2(0.0f, EditorStyle::HEADER_SIZE));
            preview_header->SetAnchorPreset(AnchorPreset::TopStretchHorizontal);
            preview_header->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
            preview_header->GetHierarchy().SetParent(preview);

            UIElement *preview_header_left = UIFactory::CreateElement();
            preview_header_left->SetAnchorPreset(AnchorPreset::StretchAll);
            preview_header_left->GetStyle().SetOpacity(0.0f);
            preview_header_left->GetHierarchy().SetParent(preview_header);
            preview_header_left->GetLayout().layout_type = LayoutType::Horizontal;
            preview_header_left->GetLayout().child_alignment = ChildAlignment::MiddleLeft;

            UIElement *preview_header_middle = UIFactory::CreateElement();
            preview_header_middle->SetAnchorPreset(AnchorPreset::StretchAll);
            preview_header_middle->GetStyle().SetOpacity(0.0f);
            preview_header_middle->GetHierarchy().SetParent(preview_header);
            preview_header_middle->GetLayout().layout_type = LayoutType::Horizontal;
            preview_header_middle->GetLayout().child_alignment = ChildAlignment::MiddleCenter;

            s_preview_container_ui_element = UIFactory::CreateElement();
            s_preview_container_ui_element->SetAnchorPreset(AnchorPreset::StretchAll);
            s_preview_container_ui_element->SetAnchorOffsetMax(Vector2(0.0f, EditorStyle::HEADER_SIZE));
            s_preview_container_ui_element->GetStyle().SetColor(EditorStyle::COLOR_HIGHLIGHT);
            s_preview_container_ui_element->GetHierarchy().SetParent(preview);

            s_preview_runtime_ui_element = UIFactory::CreateElement();
            s_preview_runtime_ui_element->SetAnchorPreset(AnchorPreset::StretchAll);
            s_preview_runtime_ui_element->GetRenderer().texture = EditorApplication::GetRenderPipeline()->GetTargetRenderTexture();
            s_preview_runtime_ui_element->GetRenderer().render_texture_attachment_index = 0;
            s_preview_runtime_ui_element->GetRenderer().enable_blending = false;
            s_preview_runtime_ui_element->GetHierarchy().SetParent(s_preview_container_ui_element);

            s_preview_editor_ui_element = UIFactory::CreateElement();
            s_preview_editor_ui_element->SetAnchorPreset(AnchorPreset::StretchAll);
            s_preview_editor_ui_element->GetRenderer().texture = EditorApplication::GetRenderPipeline()->GetEditorTargetRenderTexture();
            s_preview_editor_ui_element->GetRenderer().render_texture_attachment_index = 0;
            s_preview_editor_ui_element->GetRenderer().enable_blending = false;
            s_preview_editor_ui_element->GetHierarchy().SetParent(s_preview_container_ui_element);

            auto create_toggle = [](UIElement *parent, Font *font, const String &text, bool8 is_on, UIToggleCallback callback) -> UIToggle * {
                UIToggle *ui_toggle = UIFactory::CreateToggle();
                ui_toggle->SetSize(Vector2(25.0f, 25.0f));
                ui_toggle->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
                ui_toggle->SetToggleOnColor(EditorStyle::COLOR_HIGHLIGHT);
                ui_toggle->SetToggleOffColor(Color::White());
                ui_toggle->GetHierarchy().SetParent(parent);
                ui_toggle->GetHierarchy().RemoveChildren();
                ui_toggle->SetIsOn(is_on);
                ui_toggle->RegisterToggleCallback(callback);
                UILabel *toggle_label = UIFactory::CreateLabel();
                toggle_label->GetHierarchy().SetParent(ui_toggle);
                toggle_label->SetAnchorPreset(AnchorPreset::StretchAll);
                toggle_label->SetFont(font);
                toggle_label->SetText(text);
                toggle_label->GetStyle().GetShadow().enabled = true;
                ui_toggle->SetToggleGraphic(toggle_label);
                return ui_toggle;
            };
            auto create_button = [](UIElement *parent, Font *font, const String &text, UIButtonClickCallback callback = UIButtonClickCallback()) -> UIButton * {
                UIButton *button = UIFactory::CreateButton();
                button->SetSize(Vector2(25.0f, 25.0f));
                button->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
                button->GetHierarchy().SetParent(parent);
                UILabel *button_label = button->Q<UILabel>();
                button_label->SetFont(font);
                button_label->SetText(text);
                button_label->GetStyle().GetShadow().enabled = true;
                button->RegisterClickCallback(callback);
                return button;
            };

            // Left.
            {
                s_toggle_vsync = create_toggle(preview_header_left, s_font_icon, "\uf108", Rendering::RenderEngine::GetVSyncMode() != Rendering::VSyncMode::DontSync, [](bool8 is_on) {
                    Rendering::RenderEngine::SetVSyncMode(
                        Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync ?
                        Rendering::VSyncMode::EveryVBlank :
                        Rendering::VSyncMode::DontSync);
                });
                s_toggle_grid = create_toggle(preview_header_left, s_font_icon, "\uf850", Rendering::RenderGizmos::GetShouldDrawGrid(), [](bool8 is_on) {
                    Rendering::RenderGizmos::SetShouldDrawGrid(is_on);
                });
                s_toggle_grounds = create_toggle(preview_header_left, s_font_icon, "\uf247", Rendering::RenderGizmos::GetShouldDrawAllBounds(), [](bool8 is_on) {
                    Rendering::RenderGizmos::SetShouldDrawAllBounds(is_on);
                });
                create_button(preview_header_left, s_font_icon, "\uf03d", []() {
                    EditorCamera::Reset();
                });
            }
            
            // Middle.
            {
                UIButton *play_button = create_button(preview_header_middle, s_font_icon, "\uf04b");
                UIButton *pause_button = create_button(preview_header_middle, s_font_icon, "\uf04c");
                UIButton *stop_button = create_button(preview_header_middle, s_font_icon, "\uf04d");
                play_button->RegisterClickCallback([play_button, pause_button, stop_button]() {
                    EditorApplication::EnterRuntime();

                    play_button->GetStyle().SetColor(EditorStyle::COLOR_HIGHLIGHT);
                    play_button->SetInteractable(false);

                    pause_button->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
                    pause_button->SetInteractable(true);

                    stop_button->SetInteractable(true);
                });
                pause_button->RegisterClickCallback([play_button, pause_button, stop_button]() {
                    EditorApplication::PauseRuntime();

                    play_button->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
                    play_button->SetInteractable(true);

                    pause_button->GetStyle().SetColor(EditorStyle::COLOR_HIGHLIGHT);
                    pause_button->SetInteractable(false);

                    stop_button->SetInteractable(true);
                });
                stop_button->RegisterClickCallback([play_button, pause_button, stop_button]() {
                    EditorApplication::ExitRuntime();

                    play_button->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
                    play_button->SetInteractable(true);

                    pause_button->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
                    pause_button->SetInteractable(false);

                    stop_button->SetInteractable(false);
                });
                pause_button->SetInteractable(false);
                stop_button->SetInteractable(false);
            }

            // Right.
            {
                s_label_stats = UIFactory::CreateLabel();
                s_label_stats->SetFont(s_font_text);
                s_label_stats->SetAlignment(TextAlignment::MiddleRight);
                s_label_stats->SetAnchorPreset(AnchorPreset::StretchAll);
                s_label_stats->SetAnchorOffsetMax(Vector2(5.0f, 0.0f));
                s_label_stats->GetStyle().GetShadow().enabled = true;
                s_label_stats->GetHierarchy().SetParent(preview_header);
                s_label_stats->GetLayout().ignore_layout = true;
            }
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
            if (s_preview_editor_ui_element->GetStyle().GetVisibility() == Visibility::Visible) {
                s_preview_editor_ui_element->GetStyle().SetVisibility(Visibility::Hidden);
                s_preview_runtime_ui_element->GetStyle().SetVisibility(Visibility::Visible);
            } else {
                s_preview_editor_ui_element->GetStyle().SetVisibility(Visibility::Visible);
                s_preview_runtime_ui_element->GetStyle().SetVisibility(Visibility::Hidden);
            }
        }

        if (EditorSelection::HasSelection()) {
            World *world = EditorApplication::GetWorld();
            EntityId entity = EditorSelection::GetSelection();

            String text = StringUtils::Format("Entity:\nId: {} - Guid: {}\n\n", entity, world->GetGuid(entity).ToString());
            for (const ComponentInfo &component_info : ComponentRegistry::GetComponentInfos()) {
                void *component = world->GetComponent(component_info.id, entity);
                if (component) {
                    text += StringUtils::Format("{}\n\n", component_info.type.GetName());
                }
            }

            s_label_selection->SetText(text);
        } else {
            s_label_selection->SetText("No entity selected!");
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
        //String stats_format = "FPS: {} ({:.2f}ms) - Draw calls: {}, Vertices: {}, Triangles: {}";
        //RenderStats render_stats = Rendering::RenderEngine::GetStats();
        //String stats_title = StringUtils::Format(stats_format, Time::GetFPS(), Time::GetFrameTime(), render_stats.draw_calls, render_stats.vertex_count, render_stats.triangle_count);
        String stats_format = "FPS: {} ({:.2f}ms)";
        String stats_title = StringUtils::Format(stats_format, Time::GetFPS(), Time::GetFrameTime());
        s_label_stats->SetText(stats_title);
    }

}