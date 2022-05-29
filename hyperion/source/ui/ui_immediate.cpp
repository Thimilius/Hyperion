//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_immediate.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/loader/font_loader.hpp"
#include "hyperion/assets/utilities/text_mesh_generator.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/render/render_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  const float32 SAME_LINE_PADDING = 5.0f;
  const float32 NEXT_LINE_PADDING = 2.0f;

  using UIImmediateId = uint64;

  enum class UIImmediateWidgetFlags {
    None = 0,

    Interactable = BIT(0),
    
    DrawText = BIT(1),
    DrawShadow = BIT(2),
    DrawBackground = BIT(3),
    DrawBackgroundShadow = BIT(4)
  };
  HYP_CREATE_ENUM_FLAG_OPERATORS(UIImmediateWidgetFlags)

  struct UIImmediateElement {
    struct UIImmediateElementId {
      UIImmediateId id = 0;
      uint64 last_frame_touched_index = 0;
    } id;
    
    struct UIImmediateElementHierarchy {
      UIImmediateElement *parent = nullptr;

      UIImmediateElement *previous_sibling = nullptr;
      UIImmediateElement *next_sibling = nullptr;

      uint64 child_count = 0;
      UIImmediateElement *first_child = nullptr;
      UIImmediateElement *last_child = nullptr;  
    } hierarchy;

    struct UIImmediateElementLayout {
      UIImmediateSize semantic_size[2] = { };
      UIImmediateChildLayout child_layout = UIImmediateChildLayout::Vertical;

      float32 computed_size[2] = { };
      float32 computed_relative_position[2] = { };
      Rect rect = Rect();
    } layout;

    struct UIImmediateElementWidget {
      UIImmediateWidgetFlags flags = UIImmediateWidgetFlags::None;

      UIImmediateTheme theme = UIImmediateTheme();
      
      String text = String();
      TextAlignment text_alignment = TextAlignment::TopLeft;
    } widget;
  };

  struct UIImmediateState {
    Vector2 mouse_position = Vector2();
    bool8 is_left_mouse_down = false;
    bool8 is_left_mouse_hold = false;
    bool8 is_left_mouse_up = false;
    bool8 is_right_mouse_down = false;
    bool8 is_right_mouse_hold = false;
    bool8 is_right_mouse_up = false;
    
    UIImmediateId hovered_widget = 0;
    UIImmediateId pressed_widget = 0;

    uint64 current_frame_index = 0;

    Vector2 cursor_position = Vector2();

    Map<UIImmediateId, UIImmediateElement> elements;
    UIImmediateElement root_element;
    Array<UIImmediateElement *> element_stack;
    
    Font *font;
  };

  UIImmediateState g_state;

  //--------------------------------------------------------------
  UIImmediateId GetId(const String &text) {
    // NOTE: At some point we will need something more robust.
    // Like a id stack system of some sort.
    // Or filtering out what goes from the text into the hash with '##'.
    uint64 id = std::hash<String>()(text);
    return id;
  }
  
  //--------------------------------------------------------------
  void AdvanceCursor(Vector2 size, bool8 same_line = false) {
    if (same_line) {
      g_state.cursor_position.x += size.x + SAME_LINE_PADDING;
    } else {
      g_state.cursor_position.y -= size.y + NEXT_LINE_PADDING;  
    }
  }

  //--------------------------------------------------------------
  Vector2 ScreenPointToUISpacePoint(Vector2 screen_point) {
    float32 display_width = static_cast<float32>(Display::GetWidth());
    float32 display_height = static_cast<float32>(Display::GetHeight());
    screen_point.x -= display_width / 2.0f;
    screen_point.y -= display_height / 2.0f;
    return screen_point;
  }
  
  bool8 IsInsideRect(Rect rect, Vector2 screen_point) {
    auto is_left = [](Vector2 p0, Vector2 p1, Vector2 p2) {
      return ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y));
    };

    Vector2 min = rect.GetMin();
    Vector2 max = rect.GetMax();

    Vector2 p1 = Vector3(max.x, max.y, 0.0f);
    Vector2 p2 = Vector3(max.x, min.y, 0.0f);
    Vector2 p3 = Vector3(min.x, min.y, 0.0f);
    Vector2 p4 = Vector3(min.x, max.y, 0.0f);

    // NOTE: Counter clockwise order of points is important.
    return is_left(p1, p4, screen_point) >= 0.0f && is_left(p4, p3, screen_point) >= 0.0f && is_left(p3, p2, screen_point) >= 0.0f && is_left(p2, p1, screen_point) >= 0.0f;
  }

  //--------------------------------------------------------------
  UIImmediateElement &GetOrCreateElement(UIImmediateId id, UIImmediateWidgetFlags widget_flags) {
    UIImmediateElement *element = nullptr;

    // Try to get cached element.
    auto it = g_state.elements.Find(id);
    if (it == g_state.elements.end()) {
      UIImmediateElement new_element = { };
      new_element.id.id = id;
      new_element.widget.flags = widget_flags;
      g_state.elements.Insert(id, new_element);
      element = &g_state.elements.Get(id);
    } else {
      element = &it->second;
    }
    element->id.last_frame_touched_index = g_state.current_frame_index;

    // We have to remember to reset non persistent state.
    element->hierarchy = { };
    
    // Put element into hierarchy by appending it as a child.
    UIImmediateElement &parent = *g_state.element_stack.GetLast();
    element->hierarchy.parent = &parent;
    parent.hierarchy.child_count++;
    if (parent.hierarchy.first_child == nullptr) {
      parent.hierarchy.first_child = element;
    }
    if (parent.hierarchy.last_child != nullptr) {
      parent.hierarchy.last_child->hierarchy.next_sibling = element;
    }
    element->hierarchy.previous_sibling = parent.hierarchy.last_child; 
    parent.hierarchy.last_child = element;

    return *element;
  }

  //--------------------------------------------------------------
  UIImmediateInteraction InteractWithElement(UIImmediateElement &element) {
    if ((element.widget.flags & UIImmediateWidgetFlags::Interactable) == UIImmediateWidgetFlags::Interactable) {
      UIImmediateInteraction interaction;
      
      UIImmediateId id = element.id.id;
      
      bool8 is_inside = IsInsideRect(element.layout.rect, g_state.mouse_position);
      if (is_inside) {
        g_state.hovered_widget = id;
        if (g_state.pressed_widget == 0 && (g_state.is_left_mouse_down || g_state.is_right_mouse_down)) {
          g_state.pressed_widget = id;
        }
      }

      bool8 is_hovered_widget = g_state.hovered_widget == id;
      bool8 is_pressed_widget = g_state.pressed_widget == id;

      interaction.hovered = is_hovered_widget;
      interaction.clicked = g_state.is_left_mouse_up && is_hovered_widget && is_pressed_widget;
      interaction.right_clicked = g_state.is_right_mouse_up && is_hovered_widget && is_pressed_widget;
      
      return interaction;
    }

    return UIImmediateInteraction();
  }
  
  //--------------------------------------------------------------
  void IterateHierarchy(UIImmediateElement &parent, const std::function<void(UIImmediateElement &)> &callback) {
    callback(parent);

    UIImmediateElement *child = parent.hierarchy.first_child;
    for (uint64 i = 0; i < parent.hierarchy.child_count; ++i) {
      IterateHierarchy(*child, callback);
      child = child->hierarchy.next_sibling;
    }
  }
  
  //--------------------------------------------------------------
  void UIImmediate::Begin() {
    for (UIImmediateMesh mesh : s_meshes) {
      AssetManager::Unload(mesh.mesh);
    }
    s_meshes.Clear();

    if (!g_state.font) {
      g_state.font = FontLoader::LoadFont("data/fonts/space_mono_regular.ttf", 12, FontCharacterSet::LatinSupplement);  
    }

    g_state.mouse_position = ScreenPointToUISpacePoint(Input::GetMousePosition().ToFloat());
    g_state.is_left_mouse_down = Input::IsMouseButtonDown(MouseButtonCode::Left);
    g_state.is_left_mouse_hold = Input::IsMouseButtonHold(MouseButtonCode::Left);
    g_state.is_left_mouse_up = Input::IsMouseButtonUp(MouseButtonCode::Left);
    g_state.is_right_mouse_down = Input::IsMouseButtonDown(MouseButtonCode::Right);
    g_state.is_right_mouse_hold = Input::IsMouseButtonHold(MouseButtonCode::Right);
    g_state.is_right_mouse_up = Input::IsMouseButtonUp(MouseButtonCode::Right);
    g_state.hovered_widget = 0;
    g_state.current_frame_index++;
    g_state.cursor_position = Vector2(-static_cast<float32>(Display::GetWidth()) / 2.0f, static_cast<float32>(Display::GetHeight()) / 2.0f);

    g_state.root_element = UIImmediateElement();
    g_state.root_element.layout.semantic_size[0]= { UIImmediateSizeKind::PercentOfParent, 1.0f };
    g_state.root_element.layout.semantic_size[1]= { UIImmediateSizeKind::PercentOfParent, 1.0f };
    g_state.element_stack.Add(&g_state.root_element);
  }

  //--------------------------------------------------------------
  void UIImmediate::End() {
    g_state.element_stack.RemoveLast();
    
    Flush();

    Layout();
    Render();
    
    if (g_state.is_left_mouse_up || g_state.is_right_mouse_up) {
      g_state.pressed_widget = 0;
    }
  }

  //--------------------------------------------------------------
  void UIImmediate::BeginPanel(const String &text, UIImmediateSize size[2], UIImmediateChildLayout child_layout) {
    UIImmediateElement &element = GetOrCreateElement(GetId(text), UIImmediateWidgetFlags::DrawBackground);

    element.layout.semantic_size[0] = size[0];
    element.layout.semantic_size[1] = size[1];
    element.layout.child_layout = child_layout;

    element.widget.theme = s_panel_theme;
    
    g_state.element_stack.Add(&element);
  }

  //--------------------------------------------------------------
  void UIImmediate::EndPanel() {
    g_state.element_stack.RemoveLast();    
  }

  //--------------------------------------------------------------
  void UIImmediate::Text(const String &text) {
    UIImmediateElement &element = GetOrCreateElement(GetId(text), UIImmediateWidgetFlags::DrawText | UIImmediateWidgetFlags::DrawShadow);
    
    element.layout.semantic_size[0] = { UIImmediateSizeKind::TextContent, 0.0f };
    element.layout.semantic_size[1] = { UIImmediateSizeKind::TextContent, 0.0f };
    
    element.widget.theme = s_text_theme;
    element.widget.text = text;
    element.widget.text_alignment = TextAlignment::TopLeft;
  }

  //--------------------------------------------------------------
  UIImmediateInteraction UIImmediate::Button(const String &text) {
    UIImmediateId id = GetId(text);
    UIImmediateWidgetFlags flags = UIImmediateWidgetFlags::Interactable
      | UIImmediateWidgetFlags::DrawBackground
      | UIImmediateWidgetFlags::DrawText
      | UIImmediateWidgetFlags::DrawShadow;
    UIImmediateElement &element = GetOrCreateElement(id, flags);
    
    UIImmediateInteraction interaction = InteractWithElement(element);

    element.layout.semantic_size[0] = { UIImmediateSizeKind::TextContent, 10.0f };
    element.layout.semantic_size[1] = { UIImmediateSizeKind::TextContent, 8.0f };
    
    element.widget.theme = s_button_theme;
    element.widget.text = text;
    element.widget.text_alignment = TextAlignment::MiddleCenter;

    return interaction;
  }

  //--------------------------------------------------------------
  void UIImmediate::DrawRect(Rect rect, Color color) {
    Vector2 min = rect.GetMin();
    Vector2 max = rect.GetMax();
    
    Vector3 corners[4];
    corners[0] = Vector3(max.x, max.y, 0.0f);
    corners[1] = Vector3(max.x, min.y, 0.0f);
    corners[2] = Vector3(min.x, min.y, 0.0f);
    corners[3] = Vector3(min.x, max.y, 0.0f);
    
    s_mesh_builder.AddQuad(corners, color);
  }
  
  //--------------------------------------------------------------
  void UIImmediate::DrawText(Rect rect, const String &text, Font *font, UI::TextAlignment alignment, Color color) {
    if (text.empty()) {
      return;
    }
    
    TextMeshGenerationSettings generation_settings;
    generation_settings.text = text;
    generation_settings.font = font;
    generation_settings.alignment = alignment;
    generation_settings.color = color;
    generation_settings.rect = rect;
    generation_settings.scale = Vector2(1.0f, 1.0f);

    TextMeshGenerator::GenerateMesh(generation_settings, s_mesh_builder);
    s_mesh_builder.TransformAndAlignPixels(Matrix4x4::Identity(), Vector2Int(Display::GetWidth(), Display::GetHeight()));
  }
  
  //--------------------------------------------------------------
  void UIImmediate::Layout() {
    // Step 1: Calculate all independent size kinds likes pixels and text content.
    IterateHierarchy(g_state.root_element, [](UIImmediateElement &element) {
      auto calculate_size = [](UIImmediateElement &element, uint32 axis) {
        UIImmediateSize &semantic_size = element.layout.semantic_size[axis];
        
        if (semantic_size.kind == UIImmediateSizeKind::Pixels) {
          element.layout.computed_size[axis] = semantic_size.value;
        }
        if (semantic_size.kind == UIImmediateSizeKind::TextContent) {
          if (!element.widget.text.empty()) {
            Array<uint32> codepoints = StringUtils::GetCodepointsFromUtf8(element.widget.text);
            TextSize text_size = g_state.font->GetTextSize(codepoints, 0, 1.0f, false);
            element.layout.computed_size[axis] = text_size.size[axis] + semantic_size.value;
          }
        }
      };

      calculate_size(element, 0);
      calculate_size(element, 1);
    });

    // Step 2: Calculate all upwards dependent sizes like percentage of parent.
    IterateHierarchy(g_state.root_element, [](UIImmediateElement &element) {
      auto calculate_size = [](UIImmediateElement &element, uint32 axis) {
        if (element.layout.semantic_size[axis].kind == UIImmediateSizeKind::PercentOfParent) {
          float32 percent = Math::Clamp01(element.layout.semantic_size[axis].value);
          UIImmediateElement *parent = element.hierarchy.parent;
          float32 parent_size = 0.0f;
          if (parent == nullptr) {
            parent_size = axis == 0 ? static_cast<float32>(Display::GetWidth()) : static_cast<float32>(Display::GetHeight());
          } else {
            parent_size = parent->layout.computed_size[axis];
          }
          
          element.layout.computed_size[axis] = percent * parent_size;
        }
      };

      calculate_size(element, 0);
      calculate_size(element, 1);
    });

    // Last step: Calculate relative position based on parents child layout.
    IterateHierarchy(g_state.root_element, [](UIImmediateElement &element) {
      float32 position[2] = { };

      // Take into account the position of our parent.
      // Its position is already fully calculated as we are traversing in pre-order.
      UIImmediateElement *parent = element.hierarchy.parent;
      if (parent == nullptr) {
        
      }

      if (parent != nullptr) {
        UIImmediateElement *previous_sibling = element.hierarchy.previous_sibling;
        if (previous_sibling != nullptr) {
          switch (parent->layout.child_layout) {
            case UIImmediateChildLayout::Horizontal: {
              position[0] += previous_sibling->layout.computed_relative_position[0] + previous_sibling->layout.computed_size[0]; 
              break;
            }
            case UIImmediateChildLayout::Vertical: {
              position[1] += previous_sibling->layout.computed_relative_position[1] - previous_sibling->layout.computed_size[1];
              break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
          }
        }
      }
      
      element.layout.computed_relative_position[0] = position[0];
      element.layout.computed_relative_position[1] = position[1];

      // Move to position us properly based on our size.
      Vector2 rect_size = Vector2(element.layout.computed_size[0], element.layout.computed_size[1]);
      Vector2 rect_position = Vector2(position[0], position[1]);

      // Position (0, 0) is at the center of the screen.
      // So every element gets an offset to position it at the top left corner of the screen.
      rect_position.x -= static_cast<float32>(Display::GetWidth()) / 2.0f;
      rect_position.y += static_cast<float32>(Display::GetHeight()) / 2.0f;
      
      if (parent != nullptr) {
        rect_position.x += parent->layout.computed_relative_position[0];
        rect_position.y += parent->layout.computed_relative_position[1];
      }
      rect_position.y -= rect_size.y;
      
      element.layout.rect = Rect(rect_position, rect_size);
    });
  }

  //--------------------------------------------------------------
  void UIImmediate::Render() {
    // TODO: Color, font and shadow info should be part of a style.
    IterateHierarchy(g_state.root_element, [](UIImmediateElement &element) {
      const UIImmediateTheme &theme = element.widget.theme;
      
      bool8 draw_background_shadow = (element.widget.flags & UIImmediateWidgetFlags::DrawBackgroundShadow) == UIImmediateWidgetFlags::DrawBackgroundShadow;
      if (draw_background_shadow) {
        Rect rect = element.layout.rect;
        rect.position += theme.shadow_offset;
        DrawRect(rect, theme.shadow_color);
      }
      bool8 draw_background = (element.widget.flags & UIImmediateWidgetFlags::DrawBackground) == UIImmediateWidgetFlags::DrawBackground;
      if (draw_background) {
        bool8 is_hovered = g_state.hovered_widget == element.id.id;
        bool8 is_pressed = g_state.pressed_widget == element.id.id;

        Color color = theme.background_color;
        if (is_hovered) {
          color = theme.background_color_hover;
        }
        if (is_pressed) {
          color = theme.background_color_pressed;
        }
        
        DrawRect(element.layout.rect, color);
      }
      Flush();

      bool8 draw_shadow = (element.widget.flags & UIImmediateWidgetFlags::DrawShadow) == UIImmediateWidgetFlags::DrawShadow;
      if (draw_shadow) {
        Rect rect = element.layout.rect;
        rect.position += theme.shadow_offset;
        DrawText(rect, element.widget.text, g_state.font, element.widget.text_alignment, theme.shadow_color);
      }
      bool8 draw_text = (element.widget.flags & UIImmediateWidgetFlags::DrawText) == UIImmediateWidgetFlags::DrawText;
      if (draw_text) {
        bool8 is_hovered = g_state.hovered_widget == element.id.id;
        bool8 is_pressed = g_state.pressed_widget == element.id.id;

        Color color = theme.text_color;
        if (is_hovered) {
          color = theme.text_color_hover;
        }
        if (is_pressed) {
          color = theme.text_color_pressed;
        }
        
        DrawText(element.layout.rect, element.widget.text, g_state.font, element.widget.text_alignment, color);
      }
      Flush(AssetManager::GetMaterialPrimitive(MaterialPrimitive::Font), g_state.font->GetTexture());
    });
    
    Rendering::RenderFrameContext &render_frame_context = Rendering::RenderEngine::GetMainRenderFrame()->GetContext();
    for (UIImmediateMesh mesh : s_meshes) {
      Material *material = mesh.material ? mesh.material : AssetManager::GetMaterialPrimitive(MaterialPrimitive::UI);
      AssetId texture_id = mesh.texture
       ? mesh.texture->GetAssetInfo().id
       : AssetManager::GetTexture2DPrimitive(Texture2DPrimitive::White)->GetAssetInfo().id;
      
      Rendering::RenderFrameContextObjectUI &render_frame_context_ui_object = render_frame_context.AddEditorUIObject();
      render_frame_context_ui_object.local_to_world = Matrix4x4::Identity();
      render_frame_context_ui_object.mesh_id = mesh.mesh->GetAssetInfo().id;
      render_frame_context_ui_object.shader_id = material->GetShader()->GetAssetInfo().id;
      render_frame_context_ui_object.material_id = material->GetAssetInfo().id;
      render_frame_context_ui_object.color = mesh.color;
      render_frame_context_ui_object.texture.id = texture_id;
      render_frame_context_ui_object.texture.dimension = mesh.texture ? mesh.texture->GetDimension() : Rendering::TextureDimension::Texture2D;
      render_frame_context_ui_object.texture.render_texture_attchment_index = mesh.render_texture_attachment_index;
      render_frame_context_ui_object.enable_blending = mesh.enable_blending;
    }
  }
  
  //--------------------------------------------------------------
  void UIImmediate::Flush(Material *material, Texture *texture) {
    if (s_mesh_builder.IsEmpty()) {
      return;
    }
    
    UIImmediateMesh immediate_mesh = { };
    immediate_mesh.mesh = s_mesh_builder.CreateMesh();
    immediate_mesh.material = material ? material : AssetManager::GetMaterialPrimitive(MaterialPrimitive::UI);
    immediate_mesh.texture = texture ? texture : AssetManager::GetTexture2DPrimitive(Texture2DPrimitive::White);
    immediate_mesh.color = Color::White();
    immediate_mesh.render_texture_attachment_index = 0;
    immediate_mesh.enable_blending = true;
    s_meshes.Add(immediate_mesh);

    s_mesh_builder.Clear();
  }

}
