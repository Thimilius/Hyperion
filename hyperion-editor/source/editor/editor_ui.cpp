//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_ui.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/assets/loader/font_loader.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/time.hpp>
#include "hyperion/core/system/engine.hpp"
#include <hyperion/core/system/operating_system.hpp>
#include <hyperion/ecs/component/components/utilities/camera_utilities.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/ui/immediate/ui_immediate.hpp>
#include <hyperion/ui/immediate/ui_immediate_gizmos.hpp>

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
    UI::UIImmediateTheme *theme = UI::UIImmediate::GetDefaultTheme();
    theme->separator_color = EditorStyle::COLOR_HIGHLIGHT;
    theme->panel_color = EditorStyle::COLOR_NORMAL;
    theme->panel_color_hovered = EditorStyle::COLOR_HOVERED;
    theme->panel_color_pressed = EditorStyle::COLOR_PRESSED;
    theme->button_color = EditorStyle::COLOR_NORMAL_DARK;
    theme->button_color_hovered = EditorStyle::COLOR_HOVERED;
    theme->button_color_pressed = EditorStyle::COLOR_PRESSED;
    theme->toggle_normal_color = EditorStyle::COLOR_NORMAL_DARK;
    theme->toggle_normal_color_hovered = EditorStyle::COLOR_HOVERED;
    theme->toggle_normal_color_pressed = EditorStyle::COLOR_PRESSED;
    theme->toggle_toggled_color = EditorStyle::COLOR_HIGHLIGHT;
    theme->toggle_toggled_color_hovered = EditorStyle::COLOR_HIGHLIGHT_HOVERED;
    theme->toggle_toggled_color_pressed = EditorStyle::COLOR_HIGHLIGHT_PRESSED;
    theme->toggle_toggled_text_color = Color::White();
    theme->input_color = EditorStyle::COLOR_NORMAL_DARK;
    theme->input_color_hovered = EditorStyle::COLOR_HOVERED;
    theme->input_color_pressed = EditorStyle::COLOR_PRESSED;
    theme->input_color_focused = EditorStyle::COLOR_FOCUSED;
    theme->text_shadow_enabled = true;
    theme->font = FontLoader::LoadFont("data/fonts/space_mono_regular.ttf", EditorStyle::FONT_SIZE, FontCharacterSet::LatinSupplement);

    s_icon_theme = UI::UIImmediate::CreateTheme("Icon");
    s_icon_theme->font = FontLoader::LoadFont("data/fonts/font_awesome_solid.otf", EditorStyle::FONT_SIZE, FontCharacterSet::All);
    s_icon_theme->text_shadow_enabled = false;

    s_selection_theme = UI::UIImmediate::CreateTheme("Selection");
    s_selection_theme->panel_color = EditorStyle::COLOR_HIGHLIGHT;
    s_selection_theme->panel_color_hovered = EditorStyle::COLOR_HIGHLIGHT_HOVERED;
    s_selection_theme->panel_color_pressed = EditorStyle::COLOR_HIGHLIGHT_PRESSED;

    s_panel_theme = UI::UIImmediate::CreateTheme("Panel");
    s_panel_theme->panel_color = EditorStyle::COLOR_NORMAL;
    s_panel_theme->panel_color_hovered = EditorStyle::COLOR_NORMAL;
    s_panel_theme->panel_color_pressed = EditorStyle::COLOR_NORMAL;

    s_disabled_theme = UI::UIImmediate::CreateTheme("Disabled");
    s_disabled_theme->text_color = EditorStyle::COLOR_DISABLED;

    s_disabled_icon_theme = UI::UIImmediate::CreateTheme("Disabled Icon", s_icon_theme);
    s_disabled_icon_theme->text_color = EditorStyle::COLOR_DISABLED;
    
    s_entity_creation_menu = { {
      { "Empty", [](auto _) { EditorApplication::CreateEntity(EntityPrimitive::Base); }, { } },
      { "Empty Child", [](auto _) {
        if (EditorSelection::HasSelection()) {
          EntityId parent = EditorSelection::GetSelection();
          EntityId new_entity = EditorApplication::CreateEntity(EntityPrimitive::Base);
          EditorApplication::GetWorld()->GetHierarchy()->SetParent(new_entity, parent);
        }
      }, { } },
      { "Objects", { }, {
        { "Cube", [](auto _) { EditorApplication::CreateEntity(EntityPrimitive::Cube); }, { } },
        { "Sphere", [](auto _) { EditorApplication::CreateEntity(EntityPrimitive::Sphere); }, { } },
        { "Plane", [](auto _) { EditorApplication::CreateEntity(EntityPrimitive::Plane); }, { } },
        { "Quad", [](auto _) { EditorApplication::CreateEntity(EntityPrimitive::Quad); }, { } },
      }, },
      { "Light", { }, {
        { "Directional Light", [](auto _) { EditorApplication::CreateEntity(EntityPrimitive::DirectionalLight); }, { } },
        { "Point Light", [](auto _) { EditorApplication::CreateEntity(EntityPrimitive::PointLight); }, { } },
        { "Spot Light", [](auto _) { EditorApplication::CreateEntity(EntityPrimitive::SpotLight); }, { } },
      }, },
      { "Camera", [](auto _) { EditorApplication::CreateEntity(EntityPrimitive::Camera); }, { } }
    } };
  }

  //--------------------------------------------------------------
  void EditorUI::Update() {
    EngineState engine_state = Engine::GetEngineState();
    UIImmediate::SetOverlayColor(
      engine_state == EngineState::EditorRuntimePaused || engine_state == EngineState::EditorRuntimePlaying
      ? EditorStyle::COLOR_PLAYMODE_OVERLAY
      : Color::White()
    );
    
    UIImmediate::Begin();
    {
      HeaderPanel();

      UIImmediate::Separator();
      
      Size center_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
      UIImmediate::BeginPanel("Center Panel", center_panel_size);
      {
        Size left_panel_size[2] = { { SizeKind::PercentOfParent, 0.3f }, { SizeKind::AutoFill, 0.0f } };
        UIImmediate::BeginPanel("Left Panel", left_panel_size, ChildLayout::Vertical);
        {
          EntityHierarchyPanel();

          UIImmediate::Separator();
          
          EntityInspectionPanel();
        }
        UIImmediate::EndPanel();

        UIImmediate::Separator();
        
        Size right_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
        UIImmediate::BeginPanel("Right Panel", right_panel_size, ChildLayout::Vertical);
        {
          PreviewPanel();

          UIImmediate::Separator();
          
          Size lower_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::PercentOfParent, 0.3f } };
          UIImmediate::BeginPanel("Lower Panel", lower_panel_size);
          {
            Size lower_left_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
            UIImmediate::BeginPanel("Left Panel", lower_left_panel_size);
            {
              
            }
            UIImmediate::EndPanel();

            UIImmediate::Separator();
          
            Size lower_right_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
            UIImmediate::BeginPanel("Right Panel", lower_right_panel_size);
            {
              
            }
            UIImmediate::EndPanel();
          }
          UIImmediate::EndPanel();
        }
        UIImmediate::EndPanel();
      }
      UIImmediate::EndPanel();

      UIImmediate::Separator();
      
      Size footer_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, EditorStyle::FOOTER_HEIGHT } };
      UIImmediate::BeginPanel("Footer Panel", footer_panel_size);
      {
        
      }
      UIImmediate::EndPanel();
    }
    UIImmediate::End();

    UpdateGizmo();
  }

  //--------------------------------------------------------------
  bool8 EditorUI::HasFocusedElement() {
    return UIImmediate::HasFocusedElement();
  }

  //--------------------------------------------------------------
  Rect EditorUI::GetPreviewRect() {
    UIImmediateElement *element = UIImmediate::GetElement(s_preview_element);
    if (element == nullptr) {
      return { 0, 0, static_cast<float32>(Display::GetWidth()), static_cast<float32>(Display::GetHeight()) };  
    } else {
      return element->layout.rect;
    }
  }

  //--------------------------------------------------------------
  bool8 EditorUI::IsMouseInsidePreviewRect() {
    Vector2 mouse_position = Input::GetMousePosition().ToFloat();
    Vector2 ui_space_point = UIImmediate::ScreenPointToUISpacePoint(mouse_position);
    Rect rect = EditorUI::GetPreviewRect(); 
    return UIImmediate::IsInsideRect(rect, ui_space_point);
  }

  //--------------------------------------------------------------
  void EditorUI::HandleMouseSelection(RenderFrame *render_frame, RenderTexture *render_texture) {
    if (Input::IsMouseButtonDown(MouseButtonCode::Left) && !s_is_in_gizmo) {
      Vector2 mouse_position = Input::GetMousePosition().ToFloat();
      Vector2 ui_space_point = UIImmediate::ScreenPointToUISpacePoint(mouse_position);
      Rect rect = EditorUI::GetPreviewRect(); 
      
      if (UIImmediate::IsInsideRect(rect, ui_space_point)) {
        render_frame->DrawObjectIds(render_texture->GetRenderTargetId());

        Vector2 point = ui_space_point;
        point.x -= rect.x;
        point.y -= rect.y;
        point.x = Math::Clamp(point.x, 0.0f, rect.width - 1);
        point.y = Math::Clamp(point.y, 0.0f, rect.height - 1);

        RectInt region;
        region.position = Vector2Int(static_cast<int32>(point.x), static_cast<int32>(point.y));
        region.size = Vector2Int(1, 1);

        RenderCommandBuffer command_buffer;
        command_buffer.RequestAsyncReadback(render_texture->GetRenderTargetId(), 0, region, [](auto &result) {
          const uint32 *data = reinterpret_cast<const uint32 *>(result.data.GetData());
          if (result.data.GetLength() >= 4) {
            uint32 id = *data;
            if (id == UINT32_MAX) {
              EditorSelection::Deselect();
            } else {
              EditorSelection::Select(EntityId::FromSmall(id));
            }
          }
        });
        render_frame->ExecuteCommandBuffer(command_buffer);
      }
    }
  }

  //--------------------------------------------------------------
  void EditorUI::HeaderPanel() {
    Size header_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, EditorStyle::HEADER_HEIGHT } };
    UIImmediate::BeginPanel("Header Panel", header_panel_size);
    {
      bool8 is_translation_tool = s_transformation_tool == RenderGizmoType::Translate;
      bool8 is_rotation_tool = s_transformation_tool == RenderGizmoType::Rotate;
      bool8 is_scale_tool = s_transformation_tool == RenderGizmoType::Scale;
      if (UIImmediate::TextToggle(is_translation_tool, "\uf0b2", FitType::ToLayout, s_icon_theme).clicked) {
        s_transformation_tool = RenderGizmoType::Translate;
      }
      if (UIImmediate::TextToggle(is_rotation_tool, "\uf2f1", FitType::ToLayout, s_icon_theme).clicked) {
        s_transformation_tool = RenderGizmoType::Rotate;
      }
      if (UIImmediate::TextToggle(is_scale_tool, "\uf424", FitType::ToLayout, s_icon_theme).clicked) {
        s_transformation_tool = RenderGizmoType::Scale;
      }

      UIImmediate::BeginCenter("Play Buttons");
      {
        EngineState engine_mode = Engine::GetEngineState();
        bool8 is_playing = engine_mode == EngineState::EditorRuntimePlaying || engine_mode == EngineState::EditorRuntimePaused;
        if (UIImmediate::TextToggle(is_playing, "\uf04b", FitType::ToLayout, s_icon_theme).clicked) {
          EditorApplication::EnterRuntime();

          s_view_mode = EditorViewMode::Game;
        }
        bool8 is_paused = engine_mode == EngineState::EditorRuntimePaused;
        if (UIImmediate::TextToggle(is_paused, "\uf04c", FitType::ToLayout, s_icon_theme).clicked) {
          EditorApplication::PauseRuntime();
        }
        if (UIImmediate::Button("\uf04d", FitType::ToLayout, s_icon_theme).clicked) {
          EditorApplication::ExitRuntime();

          s_view_mode = EditorViewMode::Editor;
        }
      }
      UIImmediate::EndCenter();
    }
    UIImmediate::EndPanel();
  }

  //--------------------------------------------------------------
  void EditorUI::EntityHierarchyPanel() {
    EntityManager *manager = EditorApplication::GetWorld()->GetEntityManager();
    
    Size upper_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
    UIImmediate::BeginPanel("Upper Panel", upper_panel_size, ChildLayout::Vertical);
    {
      Size hierarchy_header_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 25.0f } };
      UIImmediate::BeginPanel("Hierarchy Header", hierarchy_header_panel_size);
      {
        if (UIImmediate::Button("\uf067", FitType::ToLayout, s_icon_theme).clicked) {
          OperatingSystem::OpenContextMenu(s_entity_creation_menu);
        }

        UIImmediate::FillSpace();

        if (UIImmediate::Button("\uf24d", FitType::ToLayout, s_icon_theme).clicked) {
          EditorApplication::DuplicateEntity();
        }
        if (UIImmediate::Button("\uf1f8", FitType::ToLayout, s_icon_theme).clicked) {
          EditorApplication::DestroyEntity();
        }
      }
      UIImmediate::EndPanel();

      UIImmediate::Separator();
            
      Size hierarchy_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
      UIImmediate::BeginPanel("Hierarchy Panel", hierarchy_panel_size, ChildLayout::Vertical);
      {
        EntityHierarchy *hierarchy = EditorApplication::GetWorld()->GetHierarchy();
        EntityId child = hierarchy->GetFirstRoot();
        for (uint64 i = 0; i < hierarchy->GetRootCount(); i++) {
          HierarchyComponent *child_hierarchy = manager->GetComponent<HierarchyComponent>(child);
          EntityHierarchyPanelRecursive(manager, child, child_hierarchy, 0);
          child = child_hierarchy->next_sibling;
        }

        if (UIImmediate::BeginPanel("Deselect Panel", hierarchy_panel_size, ChildLayout::Vertical, true, s_panel_theme).clicked) {
          EditorSelection::Deselect();
        }
        UIImmediate::EndPanel();
      }
      UIImmediate::EndPanel();
    }
    UIImmediate::EndPanel();
  }

  //--------------------------------------------------------------
  void EditorUI::EntityHierarchyPanelRecursive(EntityManager *manager, EntityId entity, HierarchyComponent *branch_hierarchy, uint32 depth) {
    String hierarchy_text;

    NameComponent *name = manager->GetComponent<NameComponent>(entity);
    
    if (name) {
      hierarchy_text += name->name;
    } else {
      hierarchy_text += "Entity";
    }

    UIImmediateTheme *theme = UIImmediate::GetDefaultTheme();
    if (EditorSelection::GetSelection() == entity) {
      theme = s_selection_theme;
    }
    
    Size panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 15 } };
    if (UIImmediate::BeginPanel(StringUtils::Format("{}", entity), panel_size, ChildLayout::Horizontal, true, theme).clicked) {
      EditorSelection::Select(entity);
    }
    {
      bool8 is_disabled = manager->HasComponent<DisabledComponent>(entity);
      
      UIImmediate::Space(SizeKind::Pixels, static_cast<float32>(depth) * 20.0f + 5.0f);
      UIImmediate::Text("\uf1b2", TextAlignment::MiddleCenter, FitType::ToLayout, false, is_disabled ? s_disabled_icon_theme : s_icon_theme);
      UIImmediate::Space(SizeKind::Pixels, 5.0f);
      UIImmediate::PushId("Name");
      UIImmediate::Text(hierarchy_text, TextAlignment::MiddleLeft, FitType::Fill, false, is_disabled ? s_disabled_theme : UIImmediate::GetDefaultTheme());
      UIImmediate::PopId();
    }
    UIImmediate::EndPanel();

    EntityId child = branch_hierarchy->first_child;
    for (uint64 i = 0; i < branch_hierarchy->child_count; i++) {
      HierarchyComponent *child_hierarchy = manager->GetComponent<HierarchyComponent>(child);
      EntityHierarchyPanelRecursive(manager, child, child_hierarchy, depth + 1);
      child = child_hierarchy->next_sibling;
    }
  }

  //--------------------------------------------------------------
  void EditorUI::EntityInspectionPanel() {
    EntityManager *manager = EditorApplication::GetWorld()->GetEntityManager();
    
    Size inspection_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
    UIImmediate::BeginPanel("Inspection Panel", inspection_panel_size, ChildLayout::Vertical);
    {
      if (EditorSelection::HasSelection()) {
        EntityId entity = EditorSelection::GetSelection();
        
        Size inspection_panel_title_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 15.0f } };
        UIImmediate::BeginPanel("Entity", inspection_panel_title_size);
        {
          UIImmediate::Text("Entity", TextAlignment::TopCenter, FitType::Fill);
        }
        UIImmediate::EndPanel();

        Size inspection_panel_header_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 25.0f } };
        UIImmediate::BeginPanel("Header", inspection_panel_header_size);
        {
          UIImmediate::Space(SizeKind::Pixels, 5.0f);
          
          String uuid_text = StringUtils::Format("UUID: {}", manager->GetUUID(entity).ToString());
          UIImmediate::Text(uuid_text, TextAlignment::MiddleLeft, FitType::ToLayout);
          
          UIImmediate::FillSpace();
          
          bool8 is_enabled = !manager->HasComponent<DisabledComponent>(entity);
          if (UIImmediate::TextToggle(is_enabled, is_enabled ? "\uf06e" : "\uf070", FitType::ToLayout, s_icon_theme).clicked) {
            manager->SetEnabled(entity, is_enabled);
          }

          bool8 is_static = manager->HasComponent<StaticComponent>(entity);
          if (UIImmediate::TextToggle(is_static,"\uf066", FitType::ToLayout, s_icon_theme).clicked) {
            manager->SetStatic(entity, is_static);
          }

          UIImmediate::Space(SizeKind::Pixels, 5.0f);
        }
        UIImmediate::EndPanel();
        
        for (const ComponentInfo &component_info : ComponentRegistry::GetComponentInfos()) {
          void *component = manager->GetComponent(component_info.id, entity);
          if (component) {
            Type component_type = *component_info.type;

            Variant hide_metadata = component_type.get_metadata(TypeMetadata::HideInEditor);
            if (hide_metadata.is_valid() && hide_metadata.is_type<bool8>()) {
              bool8 should_skip_component = hide_metadata.to_bool();
              if (should_skip_component) {
                continue;
              }
            }

            ComponentPanel(component_info, component_type, component);
          }
        }
      } else {
        String text = "No entity selected!";
        UIImmediate::Text(text, TextAlignment::TopCenter, FitType::ToLayout);  
      }
    }
    UIImmediate::EndPanel();
  }

  //--------------------------------------------------------------
  void EditorUI::PreviewPanel() {
    Size preview_container_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
    UIImmediate::BeginPanel("Preview Container", preview_container_panel_size, ChildLayout::Vertical);
    {
      Size preview_header_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 25.0f } };
      UIImmediate::BeginPanel("Preview Header", preview_header_panel_size);
      {
        bool8 is_vsync = RenderEngine::GetVSyncMode() != VSyncMode::DontSync;
        if (UIImmediate::TextToggle(is_vsync, "\uf108", FitType::ToLayout, s_icon_theme).clicked) {
          RenderEngine::SetVSyncMode(
            RenderEngine::GetVSyncMode() == VSyncMode::DontSync
              ? VSyncMode::EveryVBlank
              : VSyncMode::DontSync
          );
        }
        bool8 should_draw_grid = RenderGizmos::GetShouldDrawGrid();
        if (UIImmediate::TextToggle(should_draw_grid, "\uf850", FitType::ToLayout, s_icon_theme).clicked) {
          RenderGizmos::SetShouldDrawGrid(should_draw_grid);
        }
        bool8 should_draw_bounds = RenderGizmos::GetShouldDrawAllBounds();
        if (UIImmediate::TextToggle(should_draw_bounds, "\uf247", FitType::ToLayout, s_icon_theme).clicked) {
          RenderGizmos::SetShouldDrawAllBounds(should_draw_bounds);
        }
        if (UIImmediate::Button("\uf03d", FitType::ToLayout, s_icon_theme).clicked) {
          EditorCamera::Reset();
        }

        UIImmediate::BeginCenter("Center");
        {
          bool8 in_editor_view = s_view_mode == EditorViewMode::Editor;
          if (UIImmediate::TextToggle(in_editor_view, "\uf1b3", FitType::ToLayout, s_icon_theme).clicked) {
            s_view_mode = EditorViewMode::Editor;
          }
          bool8 in_game_view = s_view_mode == EditorViewMode::Game;
          if (UIImmediate::TextToggle(in_game_view, "\uf11b", FitType::ToLayout, s_icon_theme).clicked) {
            s_view_mode = EditorViewMode::Game;
          }
        }
        UIImmediate::EndCenter();
        
        UIImmediate::FillSpace();

        String stats_format = "FPS: {} ({:.2f}ms)";
        String stats_text = StringUtils::Format(stats_format, Time::GetFPS(), Time::GetFrameTime());
        UIImmediate::Text(stats_text, TextAlignment::MiddleCenter, FitType::ToLayout);

        UIImmediate::Space(SizeKind::Pixels, 5.0f);
      }
      UIImmediate::EndPanel();

      UIImmediate::Separator();

      EditorRenderPipeline *render_pipeline = EditorApplication::GetRenderPipeline();
      Texture *render_texture = nullptr;
      if (s_view_mode == EditorViewMode::Editor) {
        render_texture = render_pipeline->GetEditorTargetRenderTexture();
      } else {
        render_texture = render_pipeline->GetTargetRenderTexture();
      }
      
      String image_id = "Preview Image";
      Size preview_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
      UIImmediate::Image(image_id, render_texture, preview_panel_size, false);
      s_preview_element = UIImmediate::GetId(image_id);
    }
    UIImmediate::EndPanel();
  }

  //--------------------------------------------------------------
  void EditorUI::ComponentPanel(const ComponentInfo &component_info, Type component_type, void *component) {
    String component_name;
    Variant name_metadata = component_type.get_metadata(TypeMetadata::EditorName);
    if (name_metadata.is_valid() && name_metadata.is_type<String>()) {
      component_name = name_metadata.get_value<String>();
    } else {
      component_name = component_type.get_name().to_string();
    }
    
    UIImmediate::Space(SizeKind::Pixels, 10.0f);
    Size component_header_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 25.0f } };
    UIImmediate::BeginPanel(component_name + "Header", component_header_panel_size);
    {
      UIImmediate::Space(SizeKind::Pixels, 5.0f);
      
      UIImmediate::Text(component_name, TextAlignment::MiddleLeft, FitType::ToLayout);
      UIImmediate::FillSpace();
      
      if (UIImmediate::Button("\uf0e2", FitType::ToLayout, s_icon_theme).clicked) {
        component_info.destructor(component);
        component_info.constructor(component);
      }

      bool8 component_removable = true;
      Variant removable_metadata = component_type.get_metadata(TypeMetadata::EditorRemovable);
      if (removable_metadata.is_valid() && removable_metadata.is_type<bool8>()) {
        component_removable = removable_metadata.to_bool();
      }
      if (component_removable) {
        if (UIImmediate::Button("\uf1f8", FitType::ToLayout, s_icon_theme).clicked) {
          EditorApplication::GetWorld()->GetEntityManager()->RemoveComponent(component_info.id, EditorSelection::GetSelection());
        }
      }
      
      UIImmediate::Space(SizeKind::Pixels, 5.0f);
    }
    UIImmediate::EndPanel();

    Instance instance = Reflection::CreateInstanceFromRaw(component_type, component);
      
    for (Property property : component_type.get_properties()) {
      Variant serialize_metadata = property.get_metadata(PropertyMetadata::Serialize);
      if (serialize_metadata.is_valid() && serialize_metadata.is_type<bool8>()) {
        bool8 should_skip_property = !serialize_metadata.to_bool();
        if (should_skip_property) {
          continue;
        }
      }

      PropertyPanel(instance, property);
    }
  }

  //--------------------------------------------------------------
  void EditorUI::PropertyPanel(Instance instance, Property property) {
    Type property_type = property.get_type();
    String property_name = property.get_name().to_string();

    Size property_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 20.0f } };
    UIImmediate::BeginPanel(property_name, property_panel_size, ChildLayout::Horizontal);
    {
      UIImmediate::Space(SizeKind::Pixels, 15.0f);
      
      String property_label = property_name + ":";
      UIImmediate::Text(property_label, TextAlignment::MiddleCenter, FitType::ToLayout);

      if (property_type != Type::get<String>()) {
        UIImmediate::FillSpace();
      } else {
        UIImmediate::Space(SizeKind::Pixels, 10.0f);
      }
      
      Variant property_value = property.get_value(instance);

      bool8 property_set_successfully = true;
      if (property_type == Type::get<String>()) {
        String string = property_value.get_value<String>();
        if (UIImmediate::Input(property_name, string, TextAlignment::MiddleLeft, FitType::Fill).input_changed) {
          property_set_successfully = property.set_value(instance, string);
        }
      } else if (property_type.is_arithmetic()) {
        UIImmediate::Text(property_value.to_string(), TextAlignment::MiddleRight, FitType::ToLayout);
      } else if (property_type == Type::get<Vector3>()) {
        UIImmediate::Text(property_value.get_value<Vector3>().ToString(), TextAlignment::MiddleRight, FitType::ToLayout);
      } else if (property_type == Type::get<Quaternion>()) {
        UIImmediate::Text(property_value.get_value<Quaternion>().ToEulerAngles().ToString(), TextAlignment::MiddleRight, FitType::ToLayout);
      }

      if (!property_set_successfully) {
        HYP_LOG_ERROR("Editor", "Failed to set property {}!", property_name);
      }
      
      UIImmediate::Space(SizeKind::Pixels, 5.0f);
    }
    UIImmediate::EndPanel();
  }

  //--------------------------------------------------------------
  void EditorUI::UpdateGizmo() {
    RenderGizmos::SetShouldDrawTransformationGizmo(EditorSelection::HasSelection());
    if (EditorSelection::HasSelection()) {
      EntityId entity = EditorSelection::GetSelection();
      EntityManager *manager = EditorApplication::GetWorld()->GetEntityManager();
      
      LocalTransformComponent *local_transform = manager->GetComponent<LocalTransformComponent>(entity);
      DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
      DerivedTransformComponent *camera_transform = EditorCamera::GetTransform();
      CameraComponent *camera = EditorCamera::GetCamera();
      
      RenderGizmos::SetTransformationGizmoTransformation(Matrix4x4::Translate(derived_transform->position));

      // We have to remember to do everything in the space of the preview rect. That includes:
      //   - Getting the correct mouse position with (0, 0) at the bottom and corner of the preview rect
      //   - Calculating a ray from that transformed mouse position
      Rect rect = GetPreviewRect();
      Vector2 mouse_position = Input::GetMousePosition().ToFloat();
      Vector2 position = TransformScreenToPreviewPosition(mouse_position);
      Ray ray = CameraUtilities::ScreenPointToRay(camera, camera_transform, position, Vector2(rect.width, rect.height));

      GizmoManipulation manipulation = UIImmediateGizmos::Manipulate(
        s_transformation_tool,
        manager,
        entity,
        derived_transform,
        local_transform,
        camera_transform,
        ray
      );
      s_is_in_gizmo = manipulation.in_transformation;
      RenderGizmos::UpdateTransformationGizmo(s_transformation_tool, manipulation.highlight_axis);
    }
  }
  
  //--------------------------------------------------------------
  Vector2 EditorUI::TransformScreenToPreviewPosition(Vector2 screen_position) {
    Rect preview_rect = EditorUI::GetPreviewRect();

    Vector2 point = UIImmediate::ScreenPointToUISpacePoint(screen_position);
    point.x -= preview_rect.x;
    point.y -= preview_rect.y;
    
    return point;
  }
  
}
