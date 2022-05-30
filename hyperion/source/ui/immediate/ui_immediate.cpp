﻿//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/immediate/ui_immediate.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/loader/font_loader.hpp"
#include "hyperion/assets/utilities/text_mesh_generator.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/render/render_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  //--------------------------------------------------------------
  void UIImmediate::Begin() {
    for (UIImmediateMesh mesh : s_meshes) {
      AssetManager::Unload(mesh.mesh);
    }
    s_meshes.Clear();

    if (!s_default_theme.font) {
      s_default_theme.font = FontLoader::LoadFont("data/fonts/space_mono_regular.ttf", 12, FontCharacterSet::LatinSupplement);  
    }

    s_state.mouse_position = ScreenPointToUISpacePoint(Input::GetMousePosition().ToFloat());
    s_state.is_left_mouse_down = Input::IsMouseButtonDown(MouseButtonCode::Left);
    s_state.is_left_mouse_hold = Input::IsMouseButtonHold(MouseButtonCode::Left);
    s_state.is_left_mouse_up = Input::IsMouseButtonUp(MouseButtonCode::Left);
    s_state.is_right_mouse_down = Input::IsMouseButtonDown(MouseButtonCode::Right);
    s_state.is_right_mouse_hold = Input::IsMouseButtonHold(MouseButtonCode::Right);
    s_state.is_right_mouse_up = Input::IsMouseButtonUp(MouseButtonCode::Right);
    s_state.hovered_widget = 0;
    s_state.current_frame_index++;

    s_state.root_element = UIImmediateElement();
    s_state.root_element.layout.semantic_size[0]= { SizeKind::PercentOfParent, 1.0f };
    s_state.root_element.layout.semantic_size[1]= { SizeKind::PercentOfParent, 1.0f };
    s_state.element_stack.Add(&s_state.root_element);

    s_state.temporary_elements.Clear();
  }

  //--------------------------------------------------------------
  void UIImmediate::End() {
    if (s_state.element_stack.GetLength() == 1) {
      s_state.element_stack.RemoveLast();
    } else {
      HYP_LOG_ERROR("UI", s_state.element_stack.GetLength() > 1 ? "Forgot a call to UIImmediate::*End!" : "Too many calls to UIImmediate::*End!");
    }
    
    Flush();

    Layout();
    Render();

    for (auto it = s_state.persistent_elements.begin(); it != s_state.persistent_elements.end();) {
      UIImmediateElement &element = it->second; 
      element.id.looked_up_this_frame = false;
      
      if (element.id.last_frame_touched_index != s_state.current_frame_index) {
        s_state.persistent_elements.Erase(it++);
      } else {
        ++it;  
      }
    }

    if (s_state.is_left_mouse_up || s_state.is_right_mouse_up) {
      s_state.pressed_widget = 0;
    }
  }

  //--------------------------------------------------------------
  UIImmediateTheme *UIImmediate::CreateTheme(const String &name) {
    uint64 id = std::hash<String>()(name);

    if (s_themes.Contains(id)) {
      HYP_LOG_ERROR("UI", "A theme with the name '{}' already exists!", name);
    } else {
      s_themes.Insert(id, s_default_theme);  
    }
    
    return &s_themes.Get(id);
  }

  //--------------------------------------------------------------
  UIImmediateTheme *UIImmediate::GetTheme(const String &name) {
    uint64 id = std::hash<String>()(name);
    
    auto it = s_themes.Find(id);
    if (it == s_themes.end()) {
      HYP_LOG_ERROR("UI", "A theme with the name '{}' does not exists!", name);
      return nullptr;
    } else {
      return &it->second;
    }
  }
  
  //--------------------------------------------------------------
  void UIImmediate::DestroyTheme(const String &name) {
    uint64 id = std::hash<String>()(name);
    
    if (s_themes.Contains(id)) {
      s_themes.Remove(id);
    } else {
      HYP_LOG_ERROR("UI", "A theme with the name '{}' does not exists!", name);
    }
  }

  //--------------------------------------------------------------
  void UIImmediate::PushId(const String &text) {
    s_state.id_stack.Add(text);
  }

  //--------------------------------------------------------------
  void UIImmediate::PopId() {
    if (s_state.id_stack.GetLength() > 0) {
      s_state.id_stack.RemoveLast();
    } else {
      HYP_LOG_ERROR("UI", "The id stack is already empty!");
    }
  }

  //--------------------------------------------------------------
  void UIImmediate::BeginPanel(const String &text, Size size[2], ChildLayout child_layout, UIImmediateTheme *theme) {
    PushId(text);
    
    UIImmediateElement &element = GetOrCreateElement(GetId(text), UIImmediateWidgetFlags::Panel);

    element.layout.semantic_size[0] = size[0];
    element.layout.semantic_size[1] = size[1];
    element.layout.child_layout = child_layout;

    element.widget.theme = theme;
    
    s_state.element_stack.Add(&element);
  }

  //--------------------------------------------------------------
  void UIImmediate::EndPanel() {
    PopId();
    s_state.element_stack.RemoveLast();
  }

  //--------------------------------------------------------------
  void UIImmediate::BeginEmpty() {
    UIImmediateElement &element = CreateTemporaryElement(UIImmediateWidgetFlags::Empty);

    element.layout.semantic_size[0] = { SizeKind::PercentOfParent, 1.0f };
    element.layout.semantic_size[1] = { SizeKind::PercentOfParent, 1.0f };
    element.layout.child_layout = element.hierarchy.parent->layout.child_layout;

    s_state.element_stack.Add(&element);
  }

  //--------------------------------------------------------------
  void UIImmediate::EndEmpty() {
    s_state.element_stack.RemoveLast();
  }

  //--------------------------------------------------------------
  void UIImmediate::FillSpace() {
    Space(UI::SizeKind::AutoFill, 0.0f);
  }

  //--------------------------------------------------------------
  void UIImmediate::Space(SizeKind kind, float32 value) {
    UIImmediateElement &element = CreateTemporaryElement(UIImmediateWidgetFlags::Space);

    uint64 space_axis = 0;
    uint64 fill_axis = 1;
    switch (element.hierarchy.parent->layout.child_layout) {
      case ChildLayout::Horizontal: {
        space_axis = 0;
        fill_axis = 1;
        break;
      }
      case ChildLayout::Vertical: {
        space_axis = 1;
        fill_axis = 0;
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }

    element.layout.semantic_size[space_axis] = { kind, value };
    element.layout.semantic_size[fill_axis] = { SizeKind::PercentOfParent, 1.0f };
  }

  //--------------------------------------------------------------
  void UIImmediate::BeginCenter() {
    BeginEmpty();
    FillSpace();
  }

  //--------------------------------------------------------------
  void UIImmediate::EndCenter() {
    FillSpace();
    EndEmpty();
  }

  //--------------------------------------------------------------
  void UIImmediate::Separator(UIImmediateTheme *theme) {
    UIImmediateElement &element = CreateTemporaryElement(UIImmediateWidgetFlags::Separator);

    uint64 space_axis = 0;
    uint64 fill_axis = 1;
    switch (element.hierarchy.parent->layout.child_layout) {
      case ChildLayout::Horizontal: {
        space_axis = 0;
        fill_axis = 1;
        break;
      }
      case ChildLayout::Vertical: {
        space_axis = 1;
        fill_axis = 0;
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }

    element.layout.semantic_size[space_axis] = { SizeKind::Pixels, 1.0f };
    element.layout.semantic_size[fill_axis] = { SizeKind::PercentOfParent, 1.0f };
    
    element.widget.theme = theme;
  }

  //--------------------------------------------------------------
  void UIImmediate::Text(const String &text, UIImmediateTheme *theme) {
    UIImmediateElement &element = GetOrCreateElement(GetId(text), UIImmediateWidgetFlags::Text);
    
    element.layout.semantic_size[0] = { SizeKind::TextContent, 0.0f };
    element.layout.semantic_size[1] = { SizeKind::TextContent, 0.0f };
    
    element.widget.theme = theme;
    element.widget.text = text;
    element.widget.text_alignment = TextAlignment::TopLeft;
  }

  //--------------------------------------------------------------
  UIImmediateInteraction UIImmediate::Button(const String &text, UIImmediateTheme *theme) {
    UIImmediateId id = GetId(text);
    UIImmediateWidgetFlags flags = UIImmediateWidgetFlags::Button | UIImmediateWidgetFlags::Interactable;
    UIImmediateElement &element = GetOrCreateElement(id, flags);
    
    UIImmediateInteraction interaction = InteractWithElement(element);

    element.layout.semantic_size[0] = { SizeKind::TextContent, 10.0f };
    element.layout.semantic_size[1] = { SizeKind::TextContent, 8.0f };
    
    element.widget.theme = theme;
    element.widget.text = text;
    element.widget.text_alignment = TextAlignment::MiddleCenter;

    return interaction;
  }

  //--------------------------------------------------------------
  void UIImmediate::Layout() {
    // Step 1: Calculate all independent size kinds likes pixels and text content.
    IterateHierarchy(s_state.root_element, [](UIImmediateElement &element) {
      auto calculate_size = [](UIImmediateElement &element, uint32 axis) {
        Size &semantic_size = element.layout.semantic_size[axis];

        float32 computed_size = 0.0f;
        if (semantic_size.kind == SizeKind::Pixels) {
          computed_size = semantic_size.value;
        }
        if (semantic_size.kind == SizeKind::TextContent) {
          if (!element.widget.text.empty()) {
            Array<uint32> codepoints = StringUtils::GetCodepointsFromUtf8(element.widget.text);
            TextSize text_size = element.widget.theme->font->GetTextSize(codepoints, 0, 1.0f, false);
            computed_size = text_size.size[axis] + semantic_size.value;
          }
        }

        element.layout.computed_size[axis] = computed_size;

        if (element.hierarchy.parent != nullptr) {
          if ((element.widget.flags & UIImmediateWidgetFlags::Empty) != UIImmediateWidgetFlags::Empty) {
            element.hierarchy.parent->layout.computed_child_size_sum[axis] += computed_size;
          }
        }
      };

      calculate_size(element, 0);
      calculate_size(element, 1);
    });

    // Step 2: Calculate all upwards dependent sizes like percentage of parent.
    IterateHierarchy(s_state.root_element, [](UIImmediateElement &element) {
      auto calculate_size = [](UIImmediateElement &element, uint32 axis) {
        UIImmediateElement *parent = element.hierarchy.parent;
        
        // The auto fill should only be applied to the axis which is being used for child layout.
        if (element.layout.semantic_size[axis].kind == SizeKind::AutoFill) {
          if (parent != nullptr) {
            if ((axis == 0 && parent->layout.child_layout == ChildLayout::Horizontal) ||
              (axis == 1 && parent->layout.child_layout == ChildLayout::Vertical)) {
              parent->layout.fill_child_count++;  
            } else {
              // Otherwise it should act as a full percentage of parent. 
              element.layout.semantic_size[axis].kind = SizeKind::PercentOfParent;
              element.layout.semantic_size[axis].value = 1.0f;
            }
          }
        }
        
        if (element.layout.semantic_size[axis].kind == SizeKind::PercentOfParent) {
          float32 percent = Math::Clamp01(element.layout.semantic_size[axis].value);
          
          float32 parent_size = 0.0f;
          if (parent == nullptr) {
            parent_size = axis == 0 ? static_cast<float32>(Display::GetWidth()) : static_cast<float32>(Display::GetHeight());
          } else {
            parent_size = parent->layout.computed_size[axis];
          }
          float32 computed_size = percent * parent_size;

          element.layout.computed_size[axis] = computed_size;
          
          if (parent != nullptr) {
            if ((element.widget.flags & UIImmediateWidgetFlags::Empty) != UIImmediateWidgetFlags::Empty) {
              parent->layout.computed_child_size_sum[axis] += computed_size;
            }
          }  
        }
      };

      calculate_size(element, 0);
      calculate_size(element, 1);
    });

    // Step 3: Calculate fill.
    IterateHierarchy(s_state.root_element, [](UIImmediateElement &element) {
      auto calculate_size = [](UIImmediateElement &element, uint32 axis) {
        if (element.layout.semantic_size[axis].kind == SizeKind::AutoFill) {
          UIImmediateElement *parent = element.hierarchy.parent;
          if (parent != nullptr) {
            float32 parent_size = parent->layout.computed_size[axis];
            float32 computed_size = parent_size - parent->layout.computed_child_size_sum[axis];  

            if ((element.widget.flags & UIImmediateWidgetFlags::Empty) != UIImmediateWidgetFlags::Empty) {
              
            }
            element.layout.computed_size[axis] = computed_size / static_cast<float32>(parent->layout.fill_child_count);
          }
        }
      };

      calculate_size(element, 0);
      calculate_size(element, 1);
    });
    
    // Last step: Calculate relative position based on parents child layout.
    IterateHierarchy(s_state.root_element, [](UIImmediateElement &element) {
      float32 position[2] = { };

      // Position ourself next to our previous sibling depending on layout axis.
      UIImmediateElement *parent = element.hierarchy.parent;
      bool8 element_is_empty = (element.widget.flags & UIImmediateWidgetFlags::Empty) == UIImmediateWidgetFlags::Empty;
      if (parent != nullptr && !element_is_empty) {
        switch (parent->layout.child_layout) {
          case ChildLayout::Horizontal: {
            position[0] += parent->layout.child_layout_offset;
            parent->layout.child_layout_offset += element.layout.computed_size[0];
            break;
          }
          case ChildLayout::Vertical: {
            position[1] += parent->layout.child_layout_offset;
            parent->layout.child_layout_offset -= element.layout.computed_size[1];
            break;
          }
          default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
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
      
      // Take into account the position of our parent.
      // Its position is already fully calculated as we are traversing in pre-order.
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
    IterateHierarchy(s_state.root_element, [](UIImmediateElement &element) {
      const UIImmediateTheme &theme = *element.widget.theme;

      bool8 is_hovered = s_state.hovered_widget == element.id.id;
      bool8 is_pressed = s_state.pressed_widget == element.id.id;

      bool8 is_separator = (element.widget.flags & UIImmediateWidgetFlags::Separator) == UIImmediateWidgetFlags::Separator; 
      bool8 is_panel = (element.widget.flags & UIImmediateWidgetFlags::Panel) == UIImmediateWidgetFlags::Panel;
      bool8 is_text = (element.widget.flags & UIImmediateWidgetFlags::Text) == UIImmediateWidgetFlags::Text;
      bool8 is_button = (element.widget.flags & UIImmediateWidgetFlags::Button) == UIImmediateWidgetFlags::Button;
      
      if (is_separator || is_panel || is_button) {
        Color color = is_panel ? theme.panel_color : theme.button_color;
        if (is_hovered) {
          color = is_panel ? theme.panel_color_hover : theme.button_color_hover;
        }
        if (is_pressed) {
          color = is_panel ? theme.panel_color_pressed : theme.button_color_pressed;
        }

        if (is_separator) {
          color = theme.separator_color;
        }
        
        DrawRect(element.layout.rect, color);
        Flush();
      }

      if (is_text || is_button) {
        Color color = theme.text_color;
        if (is_hovered) {
          color = theme.text_color_hover;
        }
        if (is_pressed) {
          color = theme.text_color_pressed;
        }
        
        DrawText(element.layout.rect, element.widget.text, theme.font, element.widget.text_alignment, color);
        Flush(AssetManager::GetMaterialPrimitive(MaterialPrimitive::Font), theme.font->GetTexture());
      }
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

  //--------------------------------------------------------------
  UIImmediateId UIImmediate::GetId(const String &text) {
    // This is not really performant but works for now.
    String final_text;
    for (const String &id_stack_entry : s_state.id_stack) {
      final_text += id_stack_entry;
    }
    final_text += text;
    
    // NOTE: We should allow filtering out what goes from the text into the hash with '##' or similar.
    uint64 id = std::hash<String>()(final_text);
    return id;
  }
  
  //--------------------------------------------------------------
  Vector2 UIImmediate::ScreenPointToUISpacePoint(Vector2 screen_point) {
    float32 display_width = static_cast<float32>(Display::GetWidth());
    float32 display_height = static_cast<float32>(Display::GetHeight());
    screen_point.x -= display_width / 2.0f;
    screen_point.y -= display_height / 2.0f;
    return screen_point;
  }
  
  bool8 UIImmediate::IsInsideRect(Rect rect, Vector2 screen_point) {
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
    return is_left(p1, p4, screen_point) >= 0.0f
      && is_left(p4, p3, screen_point) >= 0.0f
      && is_left(p3, p2, screen_point) >= 0.0f
      && is_left(p2, p1, screen_point) >= 0.0f;
  }

  //--------------------------------------------------------------
  UIImmediateElement &UIImmediate::GetOrCreateElement(UIImmediateId id, UIImmediateWidgetFlags widget_flags) {
    UIImmediateElement *element = nullptr;

    // Try to get cached element.
    auto it = s_state.persistent_elements.Find(id);
    if (it == s_state.persistent_elements.end()) {
      UIImmediateElement new_element = { };
      new_element.id.id = id;
      new_element.widget.flags = widget_flags;
      s_state.persistent_elements.Insert(id, new_element);
      element = &s_state.persistent_elements.Get(id);
    } else {
      element = &it->second;
    }

    // This is just a validation helper to notify when an element gets called twice.
    // That will happen when the same id is used for supposedly different elements.
    if (element->id.looked_up_this_frame) {
      HYP_LOG_ERROR("UI", "Elements share an id - This leads to undefined behaviour!");
    }
    element->id.looked_up_this_frame = true;
    element->id.last_frame_touched_index = s_state.current_frame_index;

    // We have to remember to reset non persistent state.
    element->hierarchy = { };
    Rect rect = element->layout.rect;
    element->layout = { };
    element->layout.rect = rect;
    
    PlaceElementInHierarchy(*element);

    return *element;
  }

  //--------------------------------------------------------------
  UIImmediateElement &UIImmediate::CreateTemporaryElement(UIImmediateWidgetFlags widget_flags) {
    UIImmediateId temporary_id = s_state.temporary_elements.GetLength();
    
    UIImmediateElement new_element = { };
    new_element.widget.flags = widget_flags;
    s_state.temporary_elements.Insert(temporary_id, new_element);
    UIImmediateElement &element = s_state.temporary_elements.Get(temporary_id);
    PlaceElementInHierarchy(element);
    
    return element;
  }
  
  //--------------------------------------------------------------
  void UIImmediate::PlaceElementInHierarchy(UIImmediateElement &element) {
    // Put element into hierarchy by appending it as a child.
    UIImmediateElement &parent = *s_state.element_stack.GetLast();
    element.hierarchy.parent = &parent;
    parent.hierarchy.child_count++;
    if (parent.hierarchy.first_child == nullptr) {
      parent.hierarchy.first_child = &element;
    }
    if (parent.hierarchy.last_child != nullptr) {
      parent.hierarchy.last_child->hierarchy.next_sibling = &element;
    }
    element.hierarchy.previous_sibling = parent.hierarchy.last_child; 
    parent.hierarchy.last_child = &element;
  }
  
  //--------------------------------------------------------------
  UIImmediateInteraction UIImmediate::InteractWithElement(const UIImmediateElement &element) {
    if ((element.widget.flags & UIImmediateWidgetFlags::Interactable) == UIImmediateWidgetFlags::Interactable) {
      UIImmediateInteraction interaction;
      
      UIImmediateId id = element.id.id;
      
      bool8 is_inside = IsInsideRect(element.layout.rect, s_state.mouse_position);
      if (is_inside) {
        s_state.hovered_widget = id;
        if (s_state.pressed_widget == 0 && (s_state.is_left_mouse_down || s_state.is_right_mouse_down)) {
          s_state.pressed_widget = id;
        }
      }

      bool8 is_hovered_widget = s_state.hovered_widget == id;
      bool8 is_pressed_widget = s_state.pressed_widget == id;

      interaction.hovered = is_hovered_widget;
      interaction.clicked = s_state.is_left_mouse_up && is_hovered_widget && is_pressed_widget;
      interaction.right_clicked = s_state.is_right_mouse_up && is_hovered_widget && is_pressed_widget;
      
      return interaction;
    }

    return UIImmediateInteraction();
  }
  
  //--------------------------------------------------------------
  void UIImmediate::IterateHierarchy(UIImmediateElement &parent, const std::function<void(UIImmediateElement &)> &callback) {
    callback(parent);

    UIImmediateElement *child = parent.hierarchy.first_child;
    for (uint64 i = 0; i < parent.hierarchy.child_count; ++i) {
      IterateHierarchy(*child, callback);
      child = child->hierarchy.next_sibling;
    }
  }
  
}
