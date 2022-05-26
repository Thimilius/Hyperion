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
    s_font_text = FontLoader::LoadFont("data/fonts/space_mono_regular.ttf", EditorStyle::FONT_SIZE, FontCharacterSet::LatinSupplement);
    s_font_icon = FontLoader::LoadFont("data/fonts/font_awesome_solid.otf", EditorStyle::FONT_SIZE, FontCharacterSet::All);

    s_root_element = UIFactory::CreateRoot();
    s_ui_view.root_element = s_root_element;
    s_ui_view.scaling_mode = ScalingMode::ConstantPixelSize;

    auto create_toggle = [](UIElement *parent, Font *font, const String &text, bool8 is_on, UIToggleCallback callback) -> UIToggle* {
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
    auto create_button = [](UIElement *parent, Font *font, const String &text, UIButtonClickCallback callback = UIButtonClickCallback()) -> UIButton* {
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

    // Header.
    {
      UIElement *header = UIFactory::CreateElement();
      header->SetSize(Vector2(0.0f, EditorStyle::HEADER_WIDTH - 1));
      header->SetAnchorPreset(AnchorPreset::TopStretchHorizontal);
      header->GetStyle().SetColor(EditorStyle::COLOR_NORMAL_DARK);
      header->GetHierarchy().SetParent(s_root_element);

      UIElement *header_separator = UIFactory::CreateElement();
      header_separator->SetAnchorPreset(AnchorPreset::TopStretchHorizontal);
      header_separator->SetSize(Vector2(0.0f, 1.0f));
      header_separator->SetPosition(Vector2(0.0f, -static_cast<float32>(EditorStyle::HEADER_WIDTH - 1)));
      header_separator->GetStyle().SetColor(EditorStyle::COLOR_HIGHLIGHT);
      header_separator->GetHierarchy().SetParent(s_root_element);

      UIElement *header_left = UIFactory::CreateElement();
      header_left->SetAnchorPreset(AnchorPreset::StretchAll);
      header_left->GetStyle().SetOpacity(0.0f);
      header_left->GetHierarchy().SetParent(header);
      header_left->GetLayout().layout_type = LayoutType::Horizontal;
      header_left->GetLayout().child_alignment = ChildAlignment::MiddleLeft;

      UIElement *header_middle = UIFactory::CreateElement();
      header_middle->SetAnchorPreset(AnchorPreset::StretchAll);
      header_middle->GetStyle().SetOpacity(0.0f);
      header_middle->GetHierarchy().SetParent(header);
      header_middle->GetLayout().layout_type = LayoutType::Horizontal;
      header_middle->GetLayout().child_alignment = ChildAlignment::MiddleCenter;

      // Left.
      {
        create_button(header_left, s_font_icon, "\uf245", { });
        create_button(header_left, s_font_icon, "\uf0b2", { });
        create_button(header_left, s_font_icon, "\uf2f1", { });
        create_button(header_left, s_font_icon, "\uf424", { });
      }

      // Middle.
      {
        UIButton *play_button = create_button(header_middle, s_font_icon, "\uf04b");
        UIButton *pause_button = create_button(header_middle, s_font_icon, "\uf04c");
        UIButton *stop_button = create_button(header_middle, s_font_icon, "\uf04d");
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
    }

    // Hierarchy Panel.
    {
      UIElement *hierarchy_panel = UIFactory::CreateElement();
      hierarchy_panel->SetSize(Vector2(EditorStyle::LEFT_BAR_WIDTH, 0.0f));
      hierarchy_panel->SetAnchorPreset(AnchorPreset::LeftStretchVertical);
      hierarchy_panel->SetAnchorMin(Vector2(0.0f, 0.5f));
      hierarchy_panel->SetAnchorOffsetMax(Vector2(0.0f, EditorStyle::HEADER_WIDTH));
      hierarchy_panel->GetStyle().SetColor(EditorStyle::COLOR_NORMAL_DARK);
      hierarchy_panel->GetHierarchy().SetParent(s_root_element);

      s_hierarchy_container = UIFactory::CreateElement();
      s_hierarchy_container->SetAnchorPreset(AnchorPreset::StretchAll);
      s_hierarchy_container->SetAnchorOffsetMax(Vector2(3.0f, 3.0f));
      s_hierarchy_container->SetAnchorOffsetMin(Vector2(3.0f, 3.0f));
      s_hierarchy_container->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
      s_hierarchy_container->GetHierarchy().SetParent(hierarchy_panel);
    }

    // Properties Panel.
    {
      UIElement *properties_panel = UIFactory::CreateElement();
      properties_panel->SetSize(Vector2(EditorStyle::LEFT_BAR_WIDTH, 0.0f));
      properties_panel->SetAnchorPreset(AnchorPreset::LeftStretchVertical);
      properties_panel->SetAnchorMax(Vector2(0.0f, 0.5f));
      properties_panel->SetAnchorOffsetMin(Vector2(0.0f, EditorStyle::FOOTER_WIDTH));
      properties_panel->GetStyle().SetColor(EditorStyle::COLOR_NORMAL_DARK);
      properties_panel->GetHierarchy().SetParent(s_root_element);

      UIElement *properties_container = UIFactory::CreateElement();
      properties_container->SetAnchorPreset(AnchorPreset::StretchAll);
      properties_container->SetAnchorOffsetMax(Vector2(3.0f, 3.0f));
      properties_container->SetAnchorOffsetMin(Vector2(3.0f, 3.0f));
      properties_container->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
      properties_container->GetHierarchy().SetParent(properties_panel);

      s_label_properties = UIFactory::CreateLabel();
      s_label_properties->SetFont(s_font_text);
      s_label_properties->SetTextAlignment(TextAlignment::TopCenter);
      s_label_properties->SetAnchorPreset(AnchorPreset::StretchAll);
      s_label_properties->SetAnchorOffsetMax(Vector2(0.0f, 4.0f));
      s_label_properties->GetStyle().GetShadow().enabled = true;
      s_label_properties->GetHierarchy().SetParent(properties_container);
    }

    // Bottom Bar.
    {
      UIElement *bottom_bar = UIFactory::CreateElement();
      bottom_bar->SetSize(Vector2(0.0f, EditorStyle::BOTTOM_BAR_WIDTH));
      bottom_bar->SetAnchorPreset(AnchorPreset::BottomStretchHorizontal);
      bottom_bar->SetAnchorOffsetMin(Vector2(EditorStyle::LEFT_BAR_WIDTH, EditorStyle::FOOTER_WIDTH));
      bottom_bar->GetStyle().SetColor(EditorStyle::COLOR_NORMAL_DARK);
      bottom_bar->GetHierarchy().SetParent(s_root_element);

      UIElement *bottom_bar_container = UIFactory::CreateElement();
      bottom_bar_container->SetAnchorPreset(AnchorPreset::StretchAll);
      bottom_bar_container->SetAnchorOffsetMax(Vector2(3.0f, 3.0f));
      bottom_bar_container->SetAnchorOffsetMin(Vector2(3.0f, 3.0f));
      bottom_bar_container->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
      bottom_bar_container->GetHierarchy().SetParent(bottom_bar);

      UIInputField *input_field = UIFactory::CreateInputField();
      input_field->SetLineType(InputFieldLineType::MultiLine);
      input_field->SetSize(Vector2(0.0f, 150.0f));
      input_field->SetAnchorPreset(AnchorPreset::MiddleStretchHorizontal);
      input_field->SetAnchorOffsetMin(Vector2(100.0f, 0.0f));
      input_field->SetAnchorOffsetMax(Vector2(100.0f, 0.0f));
      input_field->GetStyle().SetColor(EditorStyle::COLOR_NORMAL_DARK);
      input_field->GetHierarchy().SetParent(bottom_bar_container);
      UILabel *input_label = input_field->Q<UILabel>();
      input_label->SetFont(s_font_text);
      input_label->GetStyle().SetColor(Color::White());
      input_label->GetStyle().GetShadow().enabled = true;
    }

    // Footer.
    {
      UIElement *footer = UIFactory::CreateElement();
      footer->SetSize(Vector2(0.0f, EditorStyle::FOOTER_WIDTH - 1));
      footer->SetAnchorPreset(AnchorPreset::BottomStretchHorizontal);
      footer->GetStyle().SetColor(EditorStyle::COLOR_NORMAL_DARK);
      footer->GetHierarchy().SetParent(s_root_element);

      UIElement *footer_separator = UIFactory::CreateElement();
      footer_separator->SetAnchorPreset(AnchorPreset::BottomStretchHorizontal);
      footer_separator->SetSize(Vector2(0.0f, 1.0f));
      footer_separator->SetPosition(Vector2(0.0f, static_cast<float32>(EditorStyle::FOOTER_WIDTH - 1)));
      footer_separator->GetStyle().SetColor(EditorStyle::COLOR_HIGHLIGHT);
      footer_separator->GetHierarchy().SetParent(s_root_element);
    }

    // Preview Panel.
    {
      UIElement *preview = UIFactory::CreateElement();
      preview->SetAnchorPreset(AnchorPreset::StretchAll);
      preview->SetAnchorOffsetMax(Vector2(0.0f, EditorStyle::HEADER_WIDTH));
      preview->SetAnchorOffsetMin(Vector2(EditorStyle::LEFT_BAR_WIDTH, EditorStyle::BOTTOM_BAR_WIDTH));
      preview->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
      preview->GetHierarchy().SetParent(s_root_element);

      UIElement *preview_header = UIFactory::CreateElement();
      preview_header->SetSize(Vector2(0.0f, EditorStyle::HEADER_WIDTH));
      preview_header->SetAnchorPreset(AnchorPreset::TopStretchHorizontal);
      preview_header->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
      preview_header->GetHierarchy().SetParent(preview);

      // Preview Header.
      {
        // Left.
        {
          UIElement *preview_header_left = UIFactory::CreateElement();
          preview_header_left->SetAnchorPreset(AnchorPreset::StretchAll);
          preview_header_left->GetStyle().SetOpacity(0.0f);
          preview_header_left->GetHierarchy().SetParent(preview_header);
          preview_header_left->GetLayout().layout_type = LayoutType::Horizontal;
          preview_header_left->GetLayout().child_alignment = ChildAlignment::MiddleLeft;

          s_toggle_vsync = create_toggle(preview_header_left, s_font_icon, "\uf108", Rendering::RenderEngine::GetVSyncMode() != Rendering::VSyncMode::DontSync,
                                         [](bool8 is_on) {
                                           Rendering::RenderEngine::SetVSyncMode(
                                             Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync
                                               ? Rendering::VSyncMode::EveryVBlank
                                               : Rendering::VSyncMode::DontSync);
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
          UIElement *preview_header_middle = UIFactory::CreateElement();
          preview_header_middle->SetAnchorPreset(AnchorPreset::StretchAll);
          preview_header_middle->GetStyle().SetOpacity(0.0f);
          preview_header_middle->GetHierarchy().SetParent(preview_header);
          preview_header_middle->GetLayout().layout_type = LayoutType::Horizontal;
          preview_header_middle->GetLayout().child_alignment = ChildAlignment::MiddleCenter;

          s_toggle_editor_preview = create_toggle(preview_header_middle, s_font_icon, "\uf1b3", true, { });
          s_toggle_runtime_preview = create_toggle(preview_header_middle, s_font_icon, "\uf11b", false, { });

          s_toggle_editor_preview->RegisterToggleCallback([](bool8 is_on) {
            if (is_on) {
              s_preview_editor_ui_element->GetStyle().SetVisibility(Visibility::Visible);
              s_preview_runtime_ui_element->GetStyle().SetVisibility(Visibility::Hidden);
            } else {
              s_preview_editor_ui_element->GetStyle().SetVisibility(Visibility::Hidden);
              s_preview_runtime_ui_element->GetStyle().SetVisibility(Visibility::Visible);
            }

            s_toggle_runtime_preview->Toggle(false);
          });
          s_toggle_runtime_preview->RegisterToggleCallback([](bool8 is_on) {
            if (is_on) {
              s_preview_editor_ui_element->GetStyle().SetVisibility(Visibility::Hidden);
              s_preview_runtime_ui_element->GetStyle().SetVisibility(Visibility::Visible);
            } else {
              s_preview_editor_ui_element->GetStyle().SetVisibility(Visibility::Visible);
              s_preview_runtime_ui_element->GetStyle().SetVisibility(Visibility::Hidden);
            }

            s_toggle_editor_preview->Toggle(false);
          });
        }

        // Right.
        {
          s_label_stats = UIFactory::CreateLabel();
          s_label_stats->SetFont(s_font_text);
          s_label_stats->SetTextAlignment(TextAlignment::MiddleRight);
          s_label_stats->SetAnchorPreset(AnchorPreset::StretchAll);
          s_label_stats->SetAnchorOffsetMax(Vector2(5.0f, 0.0f));
          s_label_stats->GetStyle().GetShadow().enabled = true;
          s_label_stats->GetHierarchy().SetParent(preview_header);
          s_label_stats->GetLayout().ignore_layout = true;
        }
      }

      s_preview_container_ui_element = UIFactory::CreateElement();
      s_preview_container_ui_element->SetAnchorPreset(AnchorPreset::StretchAll);
      s_preview_container_ui_element->SetAnchorOffsetMax(Vector2(0.0f, EditorStyle::HEADER_WIDTH));
      s_preview_container_ui_element->GetStyle().SetColor(EditorStyle::COLOR_HIGHLIGHT);
      s_preview_container_ui_element->GetHierarchy().SetParent(preview);

      s_preview_editor_ui_element = UIFactory::CreateElement();
      s_preview_editor_ui_element->SetAnchorPreset(AnchorPreset::StretchAll);
      s_preview_editor_ui_element->GetRenderer().texture = EditorApplication::GetRenderPipeline()->GetEditorTargetRenderTexture();
      s_preview_editor_ui_element->GetRenderer().render_texture_attachment_index = 0;
      s_preview_editor_ui_element->GetRenderer().enable_blending = false;
      s_preview_editor_ui_element->GetHierarchy().SetParent(s_preview_container_ui_element);

      s_preview_runtime_ui_element = UIFactory::CreateElement();
      s_preview_runtime_ui_element->SetAnchorPreset(AnchorPreset::StretchAll);
      s_preview_runtime_ui_element->GetRenderer().texture = EditorApplication::GetRenderPipeline()->GetTargetRenderTexture();
      s_preview_runtime_ui_element->GetRenderer().render_texture_attachment_index = 0;
      s_preview_runtime_ui_element->GetRenderer().enable_blending = false;
      s_preview_runtime_ui_element->GetHierarchy().SetParent(s_preview_container_ui_element);
      s_preview_runtime_ui_element->GetStyle().SetVisibility(Visibility::Hidden);
    }

    {
      World *world = EditorApplication::GetWorld();
      EntityManager *manager = world->GetEntityManager();

      EntityCallback created_callback;
      created_callback.Connect<EditorUI::OnEntityCreated>();
      manager->RegisterOnEntityCreated(created_callback);

      EntityCallback destroyed_callback;
      destroyed_callback.Connect<EditorUI::OnEntityDestroyed>();
      manager->RegisterOnEntityDestroyed(destroyed_callback);

      EntityId root = world->GetHierarchy()->GetFirstRoot();
      uint64 root_count = world->GetHierarchy()->GetRootCount();
      for (uint64 i = 0; i < root_count; i++) {
        HierarchyComponent *root_hierarchy = manager->GetComponent<HierarchyComponent>(root);
        UpdateHierarchyLabelBranch(manager, root, root_hierarchy, 0);
        root = root_hierarchy->next_sibling;
      }

      EditorSelectionCallback selection_callback;
      selection_callback.Connect<EditorUI::OnEditorSelection>();
      EditorSelection::RegisterSelectionCallback(selection_callback);
    }
  }

  //--------------------------------------------------------------
  void EditorUI::Update() {
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
      if (s_toggle_editor_preview->IsOn()) {
        s_toggle_runtime_preview->Toggle();
      } else {
        s_toggle_editor_preview->Toggle();
      }
    }

    UpdateStatsLabel();
    UpdateHierarchyLabel();
    UpdateSelectionLabel();

    UpdateSystems();
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
  void EditorUI::OnEntityCreated(EntityManager *manager, EntityId id) {
    HYP_TRACE("Created Entity");
  }

  //--------------------------------------------------------------
  void EditorUI::OnEntityDestroyed(EntityManager *manager, EntityId id) {
    HYP_TRACE("Destroyed Entity");
  }

  //--------------------------------------------------------------
  void EditorUI::OnEditorSelection(EntityId old_selection, EntityId new_selection) {
    if (old_selection != EntityId::EMPTY) {
      s_hierarchy.Get(old_selection)->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
    }
    if (new_selection != EntityId::EMPTY) {
      s_hierarchy.Get(new_selection)->GetStyle().SetColor(EditorStyle::COLOR_HIGHLIGHT);
    }
  }

  //--------------------------------------------------------------
  void EditorUI::UpdateStatsLabel() {
    String stats_format = "FPS: {} ({:.2f}ms)";
    String stats_title = StringUtils::Format(stats_format, Time::GetFPS(), Time::GetFrameTime());
    s_label_stats->SetText(stats_title);
  }

  //--------------------------------------------------------------
  void EditorUI::UpdateHierarchyLabel() { }

  //--------------------------------------------------------------
  void EditorUI::UpdateHierarchyLabelBranch(EntityManager *manager, EntityId branch, HierarchyComponent *branch_hierarchy, uint32 depth) {
    String hierarchy_text;

    NameComponent *name = manager->GetComponent<NameComponent>(branch);
    for (uint32 i = 0; i < depth; i++) {
      hierarchy_text += "\t";
    }

    if (name) {
      hierarchy_text += name->name;
    } else {
      hierarchy_text += "Entity";
    }

    UIButton *hierarchy_button = UIFactory::CreateButton();
    hierarchy_button->SetSize(Vector2(0.0f, 15.0f));
    hierarchy_button->SetAnchorPreset(AnchorPreset::TopStretchHorizontal);
    hierarchy_button->GetHierarchy().SetParent(s_hierarchy_container);
    hierarchy_button->GetStyle().SetColor(EditorStyle::COLOR_NORMAL);
    hierarchy_button->SetPosition(Vector2(0.0f, s_hierarchy.GetLength() * -15.0f));
    hierarchy_button->RegisterClickCallback([branch]() {
      EditorSelection::Select(branch);
    });
    UILabel *input_label = hierarchy_button->Q<UILabel>();
    input_label->SetFont(s_font_text);
    input_label->SetText(hierarchy_text);
    input_label->SetTextAlignment(TextAlignment::MiddleLeft);
    input_label->GetStyle().SetColor(Color::White());
    input_label->GetStyle().GetShadow().enabled = true;
    s_hierarchy.Insert(branch, hierarchy_button);

    EntityId child = branch_hierarchy->first_child;
    for (uint64 i = 0; i < branch_hierarchy->child_count; i++) {
      HierarchyComponent *child_hierarchy = manager->GetComponent<HierarchyComponent>(child);
      UpdateHierarchyLabelBranch(manager, child, child_hierarchy, depth + 1);
      child = child_hierarchy->next_sibling;
    }
  }

  //--------------------------------------------------------------
  void EditorUI::UpdateSelectionLabel() {
    if (EditorSelection::HasSelection()) {
      World *world = EditorApplication::GetWorld();
      EntityManager *manager = world->GetEntityManager();
      EntityId entity = EditorSelection::GetSelection();

      String text = StringUtils::Format("Entity:\nId: {} - Guid: {}\n\n", entity, manager->GetGuid(entity).ToString());
      for (const ComponentInfo &component_info : ComponentRegistry::GetComponentInfos()) {
        void *component = manager->GetComponent(component_info.id, entity);
        if (component) {
          text += StringUtils::Format("{}\n\n", component_info.type->get_name().to_string());
        }
      }

      s_label_properties->SetText(text);
    } else {
      s_label_properties->SetText("No entity selected!");
    }
  }

  //--------------------------------------------------------------
  void EditorUI::UpdateSystems() {
    UIRebuildSystem::Run(&s_ui_view);
    UIEventSystem::Run(&s_ui_view);
    RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();
    Delegate<RenderFrameContextObjectUI &()> ui_object_adder;
    ui_object_adder.Connect<&RenderFrameContext::AddEditorUIObject>(&render_frame_context);
    UIRenderSystem::Run(&s_ui_view, ui_object_adder);
  }

}
