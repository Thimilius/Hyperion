//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/immediate/ui_immediate.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/loader/font_loader.hpp"
#include "hyperion/assets/utilities/text_mesh_generator.hpp"
#include "hyperion/assets/utilities/text_utilities.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/render/render_engine.hpp"
#include "hyperion/ui/ui_element.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  //--------------------------------------------------------------
  void UIImmediate::Begin() {
    // Every mesh that is still in the cache was not used in the frame before.
    for (auto [_, mesh] : s_mesh_cache) {
      // NOTE: We could probably be a bit smarter about the unloading.
      // Maybe keeping the mesh around for a bit longer because it might actually be reused again after some time.
      // But for now unloading instantly is the safest route to prevent high memory usage. 
      AssetManager::Unload(mesh.mesh);
    }
    s_mesh_cache.clear();
    
    for (UIImmediateMesh mesh : s_meshes_in_use) { 
      s_mesh_cache.insert(std::make_pair(mesh.vertex_count, mesh));
    }
    s_meshes_in_use.Clear();
    s_mesh_draws.Clear();

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
    s_state.keys_typed = Input::GetKeysTyped();
    s_state.hovered_element = 0;
    s_state.focused_element = s_state.is_left_mouse_down || s_state.is_right_mouse_down ? 0 : s_state.focused_element; 
    s_state.current_frame_index++;

    s_state.root_element = UIImmediateElement();
    s_state.root_element.layout.semantic_size[0]= { SizeKind::Pixels, static_cast<float32>(Display::GetWidth()) };
    s_state.root_element.layout.semantic_size[1]= { SizeKind::Pixels, static_cast<float32>(Display::GetHeight()) };
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
      s_state.pressed_element = 0;
    }
  }

  //--------------------------------------------------------------
  UIImmediateTheme *UIImmediate::CreateTheme(const String &name, UIImmediateTheme *template_theme) {
    HYP_ASSERT(template_theme);

    uint64 id = std::hash<String>()(name);
    if (s_themes.Contains(id)) {
      HYP_LOG_ERROR("UI", "A theme with the name '{}' already exists!", name);
    } else {
      s_themes.Insert(id, *template_theme);  
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
  UIImmediateId UIImmediate::GetId(const String &text) {
    return HashIdText(GetIdTextFromStack(text));    
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
  bool8 UIImmediate::HasFocusedElement() {
    return s_state.focused_element != 0;
  }

  //--------------------------------------------------------------
  UIImmediateElement *UIImmediate::GetElement(UIImmediateId id) {
    auto it = s_state.persistent_elements.Find(id);
    if (it == s_state.persistent_elements.end()) {
      return nullptr;
    } else {
      return &it->second;
    }
  }

  //--------------------------------------------------------------
  UIImmediateInteraction UIImmediate::BeginPanel(const String &text, Size size[2], ChildLayout child_layout, bool8 interactable, UIImmediateTheme *theme) {
    UIImmediateWidgetFlags widget_flags = interactable ? UIImmediateWidgetFlags::Panel | UIImmediateWidgetFlags::Interactable : UIImmediateWidgetFlags::Panel;
    UIImmediateElement &element = GetOrCreateElement(text, widget_flags);

    element.layout.semantic_size[0] = size[0];
    element.layout.semantic_size[1] = size[1];
    element.layout.child_layout = child_layout;

    element.widget.theme = theme;

    PushId(text);
    s_state.element_stack.Add(&element);

    return InteractWithElement(element);
  }

  //--------------------------------------------------------------
  void UIImmediate::EndPanel() {
    PopId();
    s_state.element_stack.RemoveLast();
  }

  //--------------------------------------------------------------
  void UIImmediate::BeginEmpty(const String &id_text) {
    UIImmediateElement &element = GetOrCreateElement(id_text, UIImmediateWidgetFlags::Empty);

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

    FitLayoutAxes fit_type_axes = GetLayoutAxesForParentChildLayout(element);
    element.layout.semantic_size[fit_type_axes.fill_axis] = { SizeKind::PercentOfParent, 1.0f };
    element.layout.semantic_size[fit_type_axes.leftover_axis] = { kind, value };
  }

  //--------------------------------------------------------------
  void UIImmediate::BeginCenter(const String &id_text) {
    BeginEmpty(id_text);
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

    FitLayoutAxes fit_type_axes = GetLayoutAxesForParentChildLayout(element);
    element.layout.semantic_size[fit_type_axes.fill_axis] = { SizeKind::PercentOfParent, 1.0f };
    element.layout.semantic_size[fit_type_axes.leftover_axis] = { SizeKind::Pixels, 1.0f };
    
    element.widget.theme = theme;
  }

  //--------------------------------------------------------------
  UIImmediateInteraction UIImmediate::Text(const String &text, TextAlignment text_alignment, FitType fit_type, bool8 interactable, UIImmediateTheme *theme) {
    UIImmediateWidgetFlags widget_flags = interactable ? UIImmediateWidgetFlags::Text | UIImmediateWidgetFlags::Interactable : UIImmediateWidgetFlags::Text; 
    UIImmediateElement &element = GetOrCreateElement(text, widget_flags);

    element.layout.semantic_size[0] = { SizeKind::TextContent, 0.0f };
    element.layout.semantic_size[1] = { SizeKind::TextContent, 0.0f };
    FitToLayout(element, fit_type);
    
    element.widget.theme = theme;
    element.widget.text = text;
    element.widget.text_alignment = text_alignment;

    return InteractWithElement(element);
  }

  //--------------------------------------------------------------
  UIImmediateInteraction UIImmediate::Button(const String &text, FitType fit_type, UIImmediateTheme *theme) {
    UIImmediateWidgetFlags flags = UIImmediateWidgetFlags::Button | UIImmediateWidgetFlags::Interactable;
    UIImmediateElement &element = GetOrCreateElement(text, flags);
    
    element.layout.semantic_size[0] = { SizeKind::TextContent, 10.0f };
    element.layout.semantic_size[1] = { SizeKind::TextContent, 8.0f };
    FitToLayout(element, fit_type);
    
    element.widget.theme = theme;
    element.widget.text = text;
    element.widget.text_alignment = TextAlignment::MiddleCenter;

    return InteractWithElement(element);
  }

  //--------------------------------------------------------------
  UIImmediateInteraction UIImmediate::TextToggle(bool8 &value, const String &text, FitType fit_type, UIImmediateTheme *theme) {
    UIImmediateWidgetFlags flags = UIImmediateWidgetFlags::Toggle | UIImmediateWidgetFlags::Interactable;
    UIImmediateElement &element = GetOrCreateElement(text, flags);

    element.layout.semantic_size[0] = { SizeKind::TextContent, 10.0f };
    element.layout.semantic_size[1] = { SizeKind::TextContent, 8.0f };
    FitToLayout(element, fit_type);
    
    UIImmediateInteraction interaction = InteractWithElement(element);
    if (interaction.clicked) {
      value = !value;
    }

    element.widget.theme = theme;
    element.widget.text = text;
    element.widget.text_alignment = TextAlignment::MiddleCenter;
    element.widget.toggled = value;
    
    return interaction;
  }

  //--------------------------------------------------------------
  UIImmediateInteraction UIImmediate::Input(const String &id_text, String &text, TextAlignment text_alignment, FitType fit_type, UIImmediateTheme *theme) {
    UIImmediateWidgetFlags flags = UIImmediateWidgetFlags::Input | UIImmediateWidgetFlags::Interactable | UIImmediateWidgetFlags::Focusable;
    UIImmediateElement &element = GetOrCreateElement(id_text, flags);

    element.layout.semantic_size[0] = { SizeKind::TextContent, 10.0f };
    element.layout.semantic_size[1] = { SizeKind::TextContent, 8.0f };
    FitToLayout(element, fit_type);

    // NOTE: This currently does not take into account multiline text.
    // Furthermore we are not using the sizes for codepoints to increment/decrement the cursor position as we should.
    
    auto decrement_cursor = [](UIImmediateElement &element, int32 size) {
      element.widget.cursor_position.x -= size;
      if (element.widget.cursor_position.x < 0) {
        element.widget.cursor_position.x = 0;
      }
    };
    auto increment_cursor = [](UIImmediateElement &element, int32 size, const String &text) {
      element.widget.cursor_position.x += size;
      if (element.widget.cursor_position.x > static_cast<int32>(text.size())) {
        element.widget.cursor_position.x = static_cast<int32>(text.size());
      }
    };
    
    UIImmediateInteraction interaction = InteractWithElement(element);
    if (interaction.focused) {
      for (AppEvent *app_event : Input::GetEvents()) {
        AppEventDispatcher dispatcher = AppEventDispatcher(*app_event);
      
        dispatcher.Dispatch<KeyPressedAppEvent>([&element, &decrement_cursor, &increment_cursor, &text](KeyPressedAppEvent &event) {
          if (event.GetKeyCode() == KeyCode::Left) {
            decrement_cursor(element, 1);
          } else if (event.GetKeyCode() == KeyCode::Right) {
            increment_cursor(element, 1, text);
          }
        });  
      }
      
      for (uint64 i = 0; i < s_state.keys_typed.GetLength(); ++i) {
        String key_typed = s_state.keys_typed[i];
        bool8 has_characters = true;
        Array<uint32> codepoints = StringUtils::GetCodepointsFromUtf8(key_typed);
        for (uint32 codepoint : codepoints) {
          if (codepoint == ' ' || codepoint == '\t') {
            continue;
          } else if (codepoint == '\b') {
            if (!text.empty()) {
              uint32 codepoint_size = StringUtils::GetLastUtf8CodepointSize(text);
              int32 erase_position = element.widget.cursor_position.x - 1;
              if (erase_position >= 0) {
                text.erase(erase_position, codepoint_size);  
              }
              decrement_cursor(element, 1);
              interaction.input_changed = true;
            }
          } else if (codepoint == '\r') {
            interaction.input_submitted = true;
            // We lose focus on return.
            s_state.focused_element = 0;
          }
          
          if (!theme->font->HasCodepoint(codepoint)) {
            has_characters = false;
            break;
          }
        }

        if (has_characters) {
          text.insert(element.widget.cursor_position.x, key_typed);
          increment_cursor(element, 1, text);
          interaction.input_changed = true;
        }
      }
    }

    element.widget.theme = theme;
    element.widget.text = text;
    element.widget.text_alignment = text_alignment;

    return interaction;
  }

  //--------------------------------------------------------------
  void UIImmediate::Image(const String &id_text, Texture *texture, Size size[2], bool8 enable_blending) {
    UIImmediateElement &element = GetOrCreateElement(id_text, UIImmediateWidgetFlags::Image);
    
    element.layout.semantic_size[0] = size[0];
    element.layout.semantic_size[1] = size[1];

    element.widget.texture = texture;
    element.widget.enable_blending = enable_blending;
  }

  //--------------------------------------------------------------
  Vector2 UIImmediate::ScreenPointToUISpacePoint(Vector2 screen_point) {
    float32 display_width = static_cast<float32>(Display::GetWidth());
    float32 display_height = static_cast<float32>(Display::GetHeight());
    screen_point.x -= display_width / 2.0f;
    screen_point.y -= display_height / 2.0f;
    return screen_point;
  }

  //--------------------------------------------------------------
  Vector2 UIImmediate::UISpacePointToScreenPoint(Vector2 ui_space_point) {
    float32 display_width = static_cast<float32>(Display::GetWidth());
    float32 display_height = static_cast<float32>(Display::GetHeight());
    ui_space_point.x += display_width / 2.0f;
    ui_space_point.y += display_height / 2.0f;
    return ui_space_point;
  }

  //--------------------------------------------------------------
  bool8 UIImmediate::IsInsideRect(Rect rect, Vector2 screen_point) {
    auto is_left = [](Vector2 p0, Vector2 p1, Vector2 p2) {
      return ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y));
    };

    Vector2 min = rect.GetMin();
    min.x -= 1.0f;
    min.y += 1.0f;
    Vector2 max = rect.GetMax();
    max.x -= 1.0f;

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
            TextSize text_size =  TextUtilities::GetTextSize(element.widget.theme->font, codepoints, 0, 1.0f, false);
            computed_size = text_size.size[axis] + semantic_size.value;
          } else {
            computed_size = semantic_size.value;
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

      return true;
    });
    
    // Step 2: Calculate all upwards dependent sizes like percentage of parent and prepare fill.
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
        
        if (element.layout.semantic_size[axis].kind == SizeKind::AutoFill) {
          if (parent != nullptr) {
            float32 parent_size = parent->layout.computed_size[axis];
            float32 computed_size = parent_size - parent->layout.computed_child_size_sum[axis];
            
            element.layout.computed_size[axis] = computed_size / static_cast<float32>(parent->layout.fill_child_count);
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

      return true;
    });

    // Step 3: Calculate fill.
    IterateHierarchy(s_state.root_element, [](UIImmediateElement &element) {
      auto calculate_size = [](UIImmediateElement &element, uint32 axis) {
        if (element.layout.semantic_size[axis].kind == SizeKind::AutoFill) {
          UIImmediateElement *parent = element.hierarchy.parent;
          if (parent != nullptr) {
            float32 parent_size = parent->layout.computed_size[axis];
            float32 computed_size = parent_size - parent->layout.computed_child_size_sum[axis];
            
            element.layout.computed_size[axis] = computed_size / static_cast<float32>(parent->layout.fill_child_count);
          }
        }
      };

      calculate_size(element, 0);
      calculate_size(element, 1);

      return true;
    });
    
    // Last step: Calculate relative position to parent and final rect.
    IterateHierarchy(s_state.root_element, [](UIImmediateElement &element) {
      UIImmediateElement *parent = element.hierarchy.parent;
      
      // Position ourself relative to our previous sibling depending on the layout axis of our parent.
      float32 relative_position[2] = { };
      bool8 element_is_empty = (element.widget.flags & UIImmediateWidgetFlags::Empty) == UIImmediateWidgetFlags::Empty;
      if (parent != nullptr && !element_is_empty) {
        switch (parent->layout.child_layout) {
          case ChildLayout::Horizontal: {
            relative_position[0] += parent->layout.child_layout_offset;
            parent->layout.child_layout_offset += element.layout.computed_size[0];
            break;
          }
          case ChildLayout::Vertical: {
            relative_position[1] += parent->layout.child_layout_offset;
            parent->layout.child_layout_offset -= element.layout.computed_size[1];
            break;
          }
          default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
      }

      float32 absolute_position[2] = { relative_position[0], relative_position[1] };
      if (parent != nullptr) {
        absolute_position[0] += parent->layout.computed_absolute_position[0];
        absolute_position[1] += parent->layout.computed_absolute_position[1];

        // For now we restrict every elements size to not overflow the parents size.
        Rect parent_rect = parent->layout.rect;
        float32 x = relative_position[0];
        float32 y = relative_position[1];
        float32 width = element.layout.computed_size[0];
        float32 height = element.layout.computed_size[1];

        if (x + width > parent_rect.width) {
          element.layout.computed_size[0] = width - ((x + width) - parent_rect.width);
        }
        if (y + height > parent_rect.height) {
          element.layout.computed_size[1] = height - ((x + height) - parent_rect.height);
        }
      }
      element.layout.computed_absolute_position[0] = absolute_position[0];
      element.layout.computed_absolute_position[1] = absolute_position[1];
      
      // Properly position us in the correct coordinate space with our final calculated position and size.
      Vector2 rect_size = Vector2(element.layout.computed_size[0], element.layout.computed_size[1]);
      Vector2 rect_position = Vector2(element.layout.computed_absolute_position[0], element.layout.computed_absolute_position[1]);

      // Position (0, 0) is at the center of the screen.
      // So every element gets an offset to position it at the top left corner of the screen.
      rect_position.x -= static_cast<float32>(Display::GetWidth()) / 2.0f;
      rect_position.y += static_cast<float32>(Display::GetHeight()) / 2.0f;
      // Every rect is positioned relative to the bottom left corner.
      rect_position.y -= rect_size.y;
      
      element.layout.rect = Rect(rect_position, rect_size);
      
      return true;
    });
  }

  //--------------------------------------------------------------
  void UIImmediate::Render() {
    RectInt scissor = { 0, 0, static_cast<int32>(Display::GetWidth()), static_cast<int32>(Display::GetHeight()) };
    
    bool8 last_draw_was_a_simple_rect = false;
    IterateHierarchy(s_state.root_element, [&last_draw_was_a_simple_rect, scissor](UIImmediateElement &element) {
      if (!IsInsideParent(element)) {
        return false;
      }
      
      const UIImmediateTheme &theme = *element.widget.theme;

      bool8 is_separator = (element.widget.flags & UIImmediateWidgetFlags::Separator) == UIImmediateWidgetFlags::Separator; 
      bool8 is_panel = (element.widget.flags & UIImmediateWidgetFlags::Panel) == UIImmediateWidgetFlags::Panel;
      bool8 is_text = (element.widget.flags & UIImmediateWidgetFlags::Text) == UIImmediateWidgetFlags::Text;
      bool8 is_button = (element.widget.flags & UIImmediateWidgetFlags::Button) == UIImmediateWidgetFlags::Button;
      bool8 is_toggle = (element.widget.flags & UIImmediateWidgetFlags::Toggle) == UIImmediateWidgetFlags::Toggle;
      bool8 is_input = (element.widget.flags & UIImmediateWidgetFlags::Input) == UIImmediateWidgetFlags::Input;
      bool8 is_image = (element.widget.flags & UIImmediateWidgetFlags::Image) == UIImmediateWidgetFlags::Image;

      if (is_separator || is_panel || is_button || is_toggle || is_input || is_image) {
        Color color = GetBackgroundColor(element);

        if (is_image) {
          if (last_draw_was_a_simple_rect) {
            Flush(scissor);    
          }
          
          last_draw_was_a_simple_rect = false;
          DrawRect(element.layout.rect, color);
          Flush(scissor, AssetManager::GetMaterialPrimitive(MaterialPrimitive::UI), element.widget.texture, false);
        } else {
          DrawRect(element.layout.rect, color);
          last_draw_was_a_simple_rect = true;
        }
      }

      if (is_text || is_button || is_toggle || is_input) {
        if (last_draw_was_a_simple_rect) {
          Flush(scissor);    
        }
        
        Rect text_rect = element.layout.rect;
        
        if (theme.text_shadow_enabled) {
          Rect shadow_rect = text_rect;
          shadow_rect.position += theme.text_shadow_offset;
          DrawText(shadow_rect, element.widget.text, theme.font, element.widget.text_alignment, theme.text_shadow_color);  
        }

        Color color = GetTextColor(element);
        DrawText(text_rect, element.widget.text, theme.font, element.widget.text_alignment, color);

        Vector2 screen_point = UISpacePointToScreenPoint(Vector2(text_rect.x, text_rect.y));
        RectInt text_scissor = {
          static_cast<int32>(screen_point.x),
          static_cast<int32>(screen_point.y),
          static_cast<int32>(text_rect.width),
          static_cast<int32>(text_rect.height)
        };
        Flush(text_scissor, AssetManager::GetMaterialPrimitive(MaterialPrimitive::Font), theme.font->GetTexture());
        last_draw_was_a_simple_rect = false;
      }

      // Render cursor for input field when focused.
      if (is_input && s_state.focused_element == element.id.id) {
        Rect rect = element.layout.rect; 
        
        Array<uint32> codepoints = StringUtils::GetCodepointsFromUtf8(element.widget.text);
        Vector2 cursor_position = TextUtilities::GetCursorPosition(
          theme.font,
          codepoints,
          1.0f,
          element.widget.text_alignment,
          rect,
          element.widget.cursor_position
        );

        Rect cursor_rect = {
          cursor_position.x,
          cursor_position.y,
          1.0f,
          static_cast<float32>(theme.font->GetSize())
        };
        
        DrawRect(cursor_rect, Color::White());
        last_draw_was_a_simple_rect = true;
      }
      
      return true;
    });
    Flush(scissor);
    
    Rendering::RenderFrameContext &render_frame_context = Rendering::RenderEngine::GetMainRenderFrame()->GetContext();
    for (UIImmediateMeshDraw mesh_draw : s_mesh_draws) {
      Material *material = mesh_draw.material ? mesh_draw.material : AssetManager::GetMaterialPrimitive(MaterialPrimitive::UI);
      AssetHandle texture_handle = mesh_draw.texture
       ? mesh_draw.texture->GetMetadata().handle
       : AssetManager::GetTexture2DPrimitive(Texture2DPrimitive::White)->GetMetadata().handle;
      
      Rendering::RenderFrameContextObjectUI &render_frame_context_ui_object = render_frame_context.AddEditorUIObject();
      render_frame_context_ui_object.local_to_world = Matrix4x4::Identity();
      render_frame_context_ui_object.mesh_handle = mesh_draw.mesh.mesh->GetMetadata().handle;
      render_frame_context_ui_object.shader_handle = material->GetShader()->GetMetadata().handle;
      render_frame_context_ui_object.material_handle = material->GetMetadata().handle;
      render_frame_context_ui_object.color = mesh_draw.color;
      render_frame_context_ui_object.texture.handle = texture_handle;
      render_frame_context_ui_object.texture.dimension = mesh_draw.texture ? mesh_draw.texture->GetDimension() : Rendering::TextureDimension::Texture2D;
      render_frame_context_ui_object.texture.render_texture_attachment_index = mesh_draw.render_texture_attachment_index;
      render_frame_context_ui_object.scissor = mesh_draw.scissor;
      render_frame_context_ui_object.enable_blending = mesh_draw.enable_blending;
    }
  }

  //--------------------------------------------------------------
  Color UIImmediate::GetBackgroundColor(const UIImmediateElement &element) {
    Color result = Color::White();
    
    UIImmediateTheme &theme = *element.widget.theme;
    bool8 is_hovered = s_state.hovered_element == element.id.id;
    bool8 is_pressed = s_state.pressed_element == element.id.id;
    bool8 is_focused = s_state.focused_element == element.id.id;

    bool8 is_separator = (element.widget.flags & UIImmediateWidgetFlags::Separator) == UIImmediateWidgetFlags::Separator;
    if (is_separator) {
      result = theme.separator_color;
    }
    
    bool8 is_panel = (element.widget.flags & UIImmediateWidgetFlags::Panel) == UIImmediateWidgetFlags::Panel;
    if (is_panel) {
      result = theme.panel_color;
      if (is_hovered) {
        result = theme.panel_color_hovered;
      }
      if (is_pressed) {
        result = theme.panel_color_pressed;
      }
    }
    
    bool8 is_button = (element.widget.flags & UIImmediateWidgetFlags::Button) == UIImmediateWidgetFlags::Button;
    if (is_button) {
      result = theme.button_color;
      if (is_hovered) {
        result = theme.button_color_hovered;
      }
      if (is_pressed) {
        result = theme.button_color_pressed;
      }
    }
    
    bool8 is_toggle = (element.widget.flags & UIImmediateWidgetFlags::Toggle) == UIImmediateWidgetFlags::Toggle;
    if (is_toggle) {
      if (element.widget.toggled) {
        result = theme.toggle_toggled_color;
        if (is_hovered) {
          result = theme.toggle_toggled_color_hovered;
        }
        if (is_pressed) {
          result = theme.toggle_toggled_color_pressed;
        }
      } else {
        result = theme.toggle_normal_color;
        if (is_hovered) {
          result = theme.toggle_normal_color_hovered;
        }
        if (is_pressed) {
          result = theme.toggle_normal_color_pressed;
        }
      }
    }

    bool8 is_input = (element.widget.flags & UIImmediateWidgetFlags::Input) == UIImmediateWidgetFlags::Input;
    if (is_input) {
      result = theme.input_color;
      if (is_hovered) {
        result = theme.input_color_hovered;
      }
      if (is_pressed) {
        result = theme.input_color_pressed;
      }
      if (is_focused) {
        result = theme.input_color_focused;
      }
    }
    
    return result;
  }
  
  //--------------------------------------------------------------
  Color UIImmediate::GetTextColor(const UIImmediateElement &element) {
    Color result = Color::White();
    
    UIImmediateTheme &theme = *element.widget.theme;
    bool8 is_hovered = s_state.hovered_element == element.id.id;
    bool8 is_pressed = s_state.pressed_element == element.id.id;

    bool8 is_text = (element.widget.flags & UIImmediateWidgetFlags::Text) == UIImmediateWidgetFlags::Text;
    bool8 is_button = (element.widget.flags & UIImmediateWidgetFlags::Button) == UIImmediateWidgetFlags::Button;
    if (is_text || is_button) {
      result = theme.text_color;
      if (is_hovered) {
        result = theme.text_color_hovered;
      }
      if (is_pressed) {
        result = theme.text_color_pressed;
      }
    }
    
    bool8 is_toggle = (element.widget.flags & UIImmediateWidgetFlags::Toggle) == UIImmediateWidgetFlags::Toggle;
    if (is_toggle) {
      if (element.widget.toggled) {
        result = theme.toggle_toggled_text_color;
      } else {
        result = theme.text_color;
      }
    }

    return result;
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
    s_mesh_builder.TransformAndAlignPixels(
      Matrix4x4::Identity(),
      Vector2Int(static_cast<int32>(Display::GetWidth()), static_cast<int32>(Display::GetHeight()))
    );
  }
  
  //--------------------------------------------------------------
  void UIImmediate::Flush(RectInt scissor, Material *material, Texture *texture, bool8 affected_by_overlay) {
    if (s_mesh_builder.IsEmpty()) {
      return;
    }

    // Try to reuse a mesh that is the same size (vertex count) that we need.
    // This way we can just upload the updated data instead of creating a completely new mesh. 
    UIImmediateMesh mesh = { };
    uint32 mesh_vertex_count = s_mesh_builder.GetVertexCount();
    auto it = s_mesh_cache.find(mesh_vertex_count);
    if (it == s_mesh_cache.end()) {
      mesh.vertex_count = mesh_vertex_count;
      mesh.mesh = s_mesh_builder.CreateMesh(AssetDataAccess::Write);
    } else {
      mesh = it->second;

      // We reuse the mesh so remove it from the cache so that it can not be reused again in this frame.
      auto iterator_pair = s_mesh_cache.equal_range(mesh_vertex_count);
      auto remove_it = iterator_pair.first;
      for (; remove_it != iterator_pair.second; ++remove_it) {
        if (remove_it->second.mesh == mesh.mesh) { 
          s_mesh_cache.erase(remove_it);
          break;
        }
      }

      s_mesh_builder.SetToMesh(mesh.mesh);
    }
    s_meshes_in_use.Add(mesh);
    
    UIImmediateMeshDraw mesh_draw = { };
    mesh_draw.mesh = mesh;
    mesh_draw.material = material ? material : AssetManager::GetMaterialPrimitive(MaterialPrimitive::UI);
    mesh_draw.texture = texture ? texture : AssetManager::GetTexture2DPrimitive(Texture2DPrimitive::White);
    mesh_draw.color = affected_by_overlay ? s_overlay_color : Color::White();
    mesh_draw.render_texture_attachment_index = 0;
    mesh_draw.scissor = scissor;
    mesh_draw.enable_blending = true;
    s_mesh_draws.Add(mesh_draw);

    s_mesh_builder.Clear();
  }

  //--------------------------------------------------------------
  bool8 UIImmediate::IsInsideParent(const UIImmediateElement &element) {
    UIImmediateElement *parent = element.hierarchy.parent;
    if (parent == nullptr) {
      return true;      
    } else {
      Rect element_rect = element.layout.rect;
      Rect parent_rect = parent->layout.rect;
      return parent_rect.Intersects(element_rect);
    }
  }

  //--------------------------------------------------------------
  void UIImmediate::FitToLayout(UIImmediateElement &element, FitType fit_type) {
    switch (fit_type) {
      case FitType::ToLayout: {
        FitLayoutAxes fit_type_axes = GetLayoutAxesForParentChildLayout(element);
        element.layout.semantic_size[fit_type_axes.fill_axis] = { SizeKind::AutoFill, 0.0f };  
        break;
      }
      case FitType::Fill: {
        element.layout.semantic_size[0] = { SizeKind::AutoFill, 0.0f };
        element.layout.semantic_size[1] = { SizeKind::AutoFill, 0.0f };
        break;
      }
      case FitType::None: break;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }
  }

  //--------------------------------------------------------------
  FitLayoutAxes UIImmediate::GetLayoutAxesForParentChildLayout(const UIImmediateElement &element) {
    FitLayoutAxes axes = { };
    switch (element.hierarchy.parent->layout.child_layout) {
      case ChildLayout::Horizontal: {
        axes.fill_axis = 1;
        axes.leftover_axis = 0;
        break;
      }
      case ChildLayout::Vertical: {
        axes.fill_axis = 0;
        axes.leftover_axis = 1;
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }
    return axes;
  }
  
  //--------------------------------------------------------------
  UIImmediateId UIImmediate::HashIdText(const String &id_text) {
    // NOTE: We should allow filtering out what goes from the text into the hash with '##' or similar.
    uint64 id = std::hash<String>()(id_text);
    return id;
  }

  //--------------------------------------------------------------
  String UIImmediate::GetIdTextFromStack(const String &text) {
    // This is not really performant but works for now.
    String final_text;
    for (const String &id_stack_entry : s_state.id_stack) {
      final_text += id_stack_entry;
    }
    final_text += text;
    return final_text;
  }
  
  //--------------------------------------------------------------
  UIImmediateElement &UIImmediate::GetOrCreateElement(const String &id_text, UIImmediateWidgetFlags widget_flags) {
    UIImmediateElement *element = nullptr;

    String full_id_text = GetIdTextFromStack(id_text);
    UIImmediateId id = HashIdText(full_id_text);
    
    // Try to get cached element.
    auto it = s_state.persistent_elements.Find(id);
    if (it == s_state.persistent_elements.end()) {
      UIImmediateElement new_element = { };
      new_element.id.id = id;
      new_element.id.id_text = full_id_text;
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
  UIImmediateInteraction UIImmediate::InteractWithElement(UIImmediateElement &element) {
    if (!IsInsideParent(element)) {
      return { };
    }
      
    if ((element.widget.flags & UIImmediateWidgetFlags::Interactable) == UIImmediateWidgetFlags::Interactable) {
      UIImmediateInteraction interaction;
      
      UIImmediateId id = element.id.id;
      
      bool8 is_inside = IsInsideRect(element.layout.rect, s_state.mouse_position);
      if (is_inside) {
        s_state.hovered_element = id;
        if (s_state.pressed_element == 0 && (s_state.is_left_mouse_down || s_state.is_right_mouse_down)) {
          s_state.pressed_element = id;
          if ((element.widget.flags & UIImmediateWidgetFlags::Focusable) == UIImmediateWidgetFlags::Focusable) {
            if (s_state.focused_element != id) {
              s_state.focused_element = id;
              
              // Position the cursor at the end for an input field.
              element.widget.cursor_position = Vector2Int(static_cast<int32>(element.widget.text.size()), 0);
            }
          }
        }
      }

      bool8 is_hovered_widget = s_state.hovered_element == id;
      bool8 is_pressed_widget = s_state.pressed_element == id;
      bool8 is_focused_widget = s_state.focused_element == id;

      interaction.hovered = is_hovered_widget;
      interaction.focused = is_focused_widget;
      interaction.clicked = s_state.is_left_mouse_up && is_hovered_widget && is_pressed_widget;
      interaction.right_clicked = s_state.is_right_mouse_up && is_hovered_widget && is_pressed_widget;
      
      return interaction;
    }

    return { };
  }
  
  //--------------------------------------------------------------
  void UIImmediate::IterateHierarchy(UIImmediateElement &parent, const std::function<bool8(UIImmediateElement &)> &callback) {
    if (callback(parent)) {
      UIImmediateElement *child = parent.hierarchy.first_child;
      for (uint64 i = 0; i < parent.hierarchy.child_count; ++i) {
        IterateHierarchy(*child, callback);
        child = child->hierarchy.next_sibling;
      }  
    }
  }
  
}
