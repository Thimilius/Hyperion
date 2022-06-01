//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_ui.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/assets/loader/font_loader.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/time.hpp>
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
  }

  //--------------------------------------------------------------
  void EditorUI::Update() {
    World *world = EditorApplication::GetWorld();
    EntityManager *manager = world->GetEntityManager();
    EntityHierarchy *hierarchy = world->GetHierarchy();
    UIImmediateTheme *icon_theme = UIImmediate::GetTheme("Icon");

    if (Input::IsKeyDown(KeyCode::F2)) {
      RenderEngine::SetVSyncMode(
        RenderEngine::GetVSyncMode() == VSyncMode::DontSync
          ? VSyncMode::EveryVBlank
          : VSyncMode::DontSync
      );
    }
    if (Input::IsKeyDown(KeyCode::F3)) {
      RenderGizmos::SetShouldDrawGrid(!RenderGizmos::GetShouldDrawGrid());
    }
    if (Input::IsKeyDown(KeyCode::F4)) {
      RenderGizmos::SetShouldDrawAllBounds(!RenderGizmos::GetShouldDrawAllBounds());
    }
    if (Input::IsKeyDown(KeyCode::Delete)) {
      if (EditorSelection::HasSelection()) {
        manager->DestroyEntity(EditorSelection::GetSelection());
        EditorSelection::Deselect();
      }
    }
    
    UIImmediate::Begin();
    {
      Size header_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 25.0f } };
      UIImmediate::BeginPanel("Header Panel", header_panel_size);
      {
        static bool8 translation_tool = true;
        static bool8 rotation_tool = false;
        static bool8 scale_tool = false;
        
        if (UIImmediate::TextToggle(translation_tool, "\uf0b2", FitType::ToLayout, icon_theme).clicked) {
          translation_tool = true;
          rotation_tool = false;
          scale_tool = false;
        }
        if (UIImmediate::TextToggle(rotation_tool, "\uf2f1", FitType::ToLayout, icon_theme).clicked) {
          translation_tool = false;
          rotation_tool = true;
          scale_tool = false;
        }
        if (UIImmediate::TextToggle(scale_tool, "\uf424", FitType::ToLayout, icon_theme).clicked) {
          translation_tool = false;
          rotation_tool = false;
          scale_tool = true;
        }

        UIImmediate::BeginCenter("Play Buttons");
        {
          if (UIImmediate::Button("\uf04b", FitType::ToLayout, icon_theme).clicked) {
            
          }
          if (UIImmediate::Button("\uf04c", FitType::ToLayout, icon_theme).clicked) {
            
          }
          if (UIImmediate::Button("\uf04d", FitType::ToLayout, icon_theme).clicked) {
            
          }
        }
        UIImmediate::EndCenter();

        UIImmediate::FillSpace();

        if (UIImmediate::Button("Reset Window", FitType::ToLayout).clicked) {
          EditorApplication::GetInstance()->GetMainWindow()->SetSize(1280, 720);
        }
      }
      UIImmediate::EndPanel();

      UIImmediate::Separator();
      
      Size center_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
      UIImmediate::BeginPanel("Center Panel", center_panel_size);
      {
        Size left_panel_size[2] = { { SizeKind::PercentOfParent, 0.3f }, { SizeKind::AutoFill, 0.0f } };
        UIImmediate::BeginPanel("Left Panel", left_panel_size, ChildLayout::Vertical);
        {
          Size upper_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
          UIImmediate::BeginPanel("Upper Panel", upper_panel_size, ChildLayout::Vertical);
          {
            Size hierarchy_header_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 25.0f } };
            UIImmediate::BeginPanel("Hierarchy Header", hierarchy_header_panel_size);
            {
              if (UIImmediate::Button("\uf067", FitType::ToLayout, icon_theme).clicked) {
                manager->CreateEntity();
              }

              UIImmediate::FillSpace();
              
              if (UIImmediate::Button("\uf1f8", FitType::ToLayout, icon_theme).clicked) {
                if (EditorSelection::HasSelection()) {
                  manager->DestroyEntity(EditorSelection::GetSelection());
                  EditorSelection::Deselect();
                }
              }
            }
            UIImmediate::EndPanel();

            UIImmediate::Separator();
            
            Size hierarchy_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
            UIImmediate::BeginPanel("Hierarchy Panel", hierarchy_panel_size, ChildLayout::Vertical);
            {
              EntityId child = hierarchy->GetFirstRoot();
              for (uint64 i = 0; i < hierarchy->GetRootCount(); i++) {
                HierarchyComponent *child_hierarchy = manager->GetComponent<HierarchyComponent>(child);
                DrawEntityHierarchy(manager, child, child_hierarchy, 0);
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

          UIImmediate::Separator();
          
          Size lower_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
          UIImmediate::BeginPanel("Inspection Panel", lower_panel_size, ChildLayout::Vertical);
          {
            if (EditorSelection::HasSelection()) {
              EntityId entity = EditorSelection::GetSelection();
              String text = StringUtils::Format(
                "Entity:\nId: Index: {} - Version: {}\n Guid: {}",
                EntityUtilities::GetIndex(entity),
                EntityUtilities::GetVersion(entity),
                manager->GetGuid(entity).ToString()
              );
              UIImmediate::Text(text, TextAlignment::TopCenter, FitType::ToLayout);
              UIImmediate::Space(SizeKind::Pixels, 10);
              
              for (const ComponentInfo &component_info : ComponentRegistry::GetComponentInfos()) {
                void *component = manager->GetComponent(component_info.id, entity);
                if (component) {
                  Type component_type = *component_info.type;
                  UIImmediate::Space(SizeKind::Pixels, 10);
                  UIImmediate::Text(component_type.get_name().to_string(), TextAlignment::MiddleCenter, FitType::ToLayout);
                  
                  if (component_type == Type::get<NameComponent>()) {
                    Instance instance = Reflection::CreateInstanceFromRaw(component_type, component);
                    
                    for (Property property : component_type.get_properties()) {
                      Type property_type = property.get_type();
                      String property_name = property.get_name().to_string();
                      
                      Size property_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 20.0f } };
                      UIImmediate::BeginPanel(property_name, property_panel_size, ChildLayout::Horizontal);
                      {
                        String property_label = property_name + ":";
                        UIImmediate::Text(property_label, TextAlignment::MiddleCenter, FitType::ToLayout);
                          
                        if (property_type == Type::get<String>()) {
                          String string = property.get_value(instance).get_value<String>();
                          if (UIImmediate::Input(property_name, string, TextAlignment::MiddleLeft, FitType::Fill).input_changed) {
                            property.set_value(instance, string);
                          }
                        }
                      }
                      UIImmediate::EndPanel();
                    }
                  }
                }
              }
            } else {
              String text = "No entity selected!";
              UIImmediate::Text(text, TextAlignment::TopCenter, FitType::ToLayout);  
            }
          }
          UIImmediate::EndPanel();
        }
        UIImmediate::EndPanel();

        UIImmediate::Separator();
        
        Size right_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
        UIImmediate::BeginPanel("Right Panel", right_panel_size, ChildLayout::Vertical);
        {
          Size preview_container_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
          UIImmediate::BeginPanel("Preview Container", preview_container_panel_size, ChildLayout::Vertical);
          {
            Size preview_header_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 25.0f } };
            UIImmediate::BeginPanel("Preview Header", preview_header_panel_size);
            {
              bool8 is_vsync = RenderEngine::GetVSyncMode() != VSyncMode::DontSync;
              if (UIImmediate::TextToggle(is_vsync, "\uf108", FitType::ToLayout, icon_theme).clicked) {
                RenderEngine::SetVSyncMode(
                  RenderEngine::GetVSyncMode() == VSyncMode::DontSync
                    ? VSyncMode::EveryVBlank
                    : VSyncMode::DontSync
                );
              }
              bool8 should_draw_grid = RenderGizmos::GetShouldDrawGrid();
              if (UIImmediate::TextToggle(should_draw_grid, "\uf850", FitType::ToLayout, icon_theme).clicked) {
                RenderGizmos::SetShouldDrawGrid(should_draw_grid);
              }
              bool8 should_draw_bounds = RenderGizmos::GetShouldDrawAllBounds();
              if (UIImmediate::TextToggle(should_draw_bounds, "\uf247", FitType::ToLayout, icon_theme).clicked) {
                RenderGizmos::SetShouldDrawAllBounds(should_draw_bounds);
              }
              if (UIImmediate::Button("\uf03d", FitType::ToLayout, icon_theme).clicked) {
                EditorCamera::Reset();
              }
              
              UIImmediate::FillSpace();

              String stats_format = "FPS: {} ({:.2f}ms)";
              String stats_text = StringUtils::Format(stats_format, Time::GetFPS(), Time::GetFrameTime());
              UIImmediate::Text(stats_text, TextAlignment::MiddleCenter, FitType::ToLayout);

              UIImmediate::Space(SizeKind::Pixels, 5.0f);
            }
            UIImmediate::EndPanel();

            UIImmediate::Separator();

            String image_id = "Preview Image";
            Texture *texture = EditorApplication::GetRenderPipeline()->GetEditorTargetRenderTexture();
            s_preview_element = UIImmediate::GetId(image_id);
            Size preview_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
            UIImmediate::Image(image_id, texture, preview_panel_size, false);
          }
          UIImmediate::EndPanel();

          UIImmediate::Separator();
          
          Size lower_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::PercentOfParent, 0.3f } };
          UIImmediate::BeginPanel("Lower Panel", lower_panel_size);
          {
            Size left_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
            UIImmediate::BeginPanel("Left Panel", left_panel_size);
            {
              
            }
            UIImmediate::EndPanel();

            UIImmediate::Separator();
          
            Size right_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
            UIImmediate::BeginPanel("Right Panel", right_panel_size);
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
      
      Size footer_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 25.0f } };
      UIImmediate::BeginPanel("Footer Panel", footer_panel_size);
      {
        
      }
      UIImmediate::EndPanel();
    }
    UIImmediate::End();

    if (EditorSelection::HasSelection()) {
      EntityId entity = EditorSelection::GetSelection();
      LocalTransformComponent *local_transform = manager->GetComponent<LocalTransformComponent>(entity);
      DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
      DerivedTransformComponent *camera_transform = EditorCamera::GetTransform();
      CameraComponent *camera = EditorCamera::GetCamera();

      // We have to remember to do everything in the space of the preview rect. That includes:
      //   - Getting the correct mouse position with (0, 0) at the bottom and corner of the preview rect
      //   - Calculating a ray from that transformed mouse position
      Rect rect = GetPreviewRect();
      Vector2 mouse_position = Input::GetMousePosition().ToFloat();
      Vector2 position = TransformScreenToPreviewPosition(mouse_position);
      Ray ray = CameraUtilities::ScreenPointToRay(camera, camera_transform, position, Vector2(rect.width, rect.height));

      s_is_in_gizmo = false;
      if (UIImmediateGizmos::Manipulate(derived_transform, local_transform, camera_transform, ray)) {
        s_is_in_gizmo = true;
      }
    }
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
              EditorSelection::Select(EntityUtilities::CreateFromSmallId(id));
            }
          }
        });
        render_frame->ExecuteCommandBuffer(command_buffer);
      }
    }
  }

  //--------------------------------------------------------------
  Vector2 EditorUI::TransformScreenToPreviewPosition(Vector2 screen_position) {
    // NOTE: This is local for the preview rect!
    Rect preview_rect = EditorUI::GetPreviewRect();

    Vector2 ui_space_point = UIImmediate::ScreenPointToUISpacePoint(screen_position);
    Vector2 point = ui_space_point;
    point.x -= preview_rect.x;
    point.y -= preview_rect.y;
    
    float32 preview_rect_width = preview_rect.width;
    float32 preview_rect_height = preview_rect.height;
    //point.x -= preview_rect_width / 2.0f;
    //point.y -= preview_rect_height / 2.0f;
    
    return point;
  }
  
  //--------------------------------------------------------------
  void EditorUI::DrawEntityHierarchy(EntityManager *manager, EntityId entity, HierarchyComponent *branch_hierarchy, uint32 depth) {
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
      UIImmediate::Space(SizeKind::Pixels, depth * 20.0f + 5.0f);
      UIImmediate::Text("\uf1b2", TextAlignment::MiddleCenter, FitType::ToLayout, false, s_icon_theme);
      UIImmediate::Space(SizeKind::Pixels, 5.0f);
      UIImmediate::PushId("Name");
      UIImmediate::Text(hierarchy_text, TextAlignment::MiddleLeft, FitType::Fill);
      UIImmediate::PopId();
    }
    UIImmediate::EndPanel();

    EntityId child = branch_hierarchy->first_child;
    for (uint64 i = 0; i < branch_hierarchy->child_count; i++) {
      HierarchyComponent *child_hierarchy = manager->GetComponent<HierarchyComponent>(child);
      DrawEntityHierarchy(manager, child, child_hierarchy, depth + 1);
      child = child_hierarchy->next_sibling;
    }
  }

}
