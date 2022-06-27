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
#include "hyperion/core/app/time.hpp"
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
    s_state.mouse_scroll = Input::GetMouseScroll();
    s_state.is_left_mouse_down = Input::IsMouseButtonDown(MouseButtonCode::Left);
    s_state.is_left_mouse_hold = Input::IsMouseButtonHold(MouseButtonCode::Left);
    s_state.is_left_mouse_up = Input::IsMouseButtonUp(MouseButtonCode::Left);
    s_state.is_right_mouse_down = Input::IsMouseButtonDown(MouseButtonCode::Right);
    s_state.is_right_mouse_hold = Input::IsMouseButtonHold(MouseButtonCode::Right);
    s_state.is_right_mouse_up = Input::IsMouseButtonUp(MouseButtonCode::Right);
    s_state.hovered_element = 0;
    s_state.last_focused_element = s_state.focused_element;
    s_state.focused_element = s_state.is_left_mouse_down || s_state.is_right_mouse_down || Input::IsMouseButtonDown(MouseButtonCode::Middle)
      ? 0
      : s_state.focused_element;
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
  UIImmediateInteraction UIImmediate::BeginPanel(
    const String &text,
    Size size[2],
    ChildLayout child_layout,
    bool8 scrollable,
    bool8 interactable,
    UIImmediateTheme *theme) {
    UIImmediateWidgetFlags widget_flags = interactable ? UIImmediateWidgetFlags::Panel | UIImmediateWidgetFlags::Interactable : UIImmediateWidgetFlags::Panel;
    if (scrollable) {
      widget_flags |= UIImmediateWidgetFlags::Scrollable;
    }
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

    // NOTE: Cursor position is a byte offset into the string not a unicode character offset.
    // That means every modification we make to the cursor has to be converted to the appropriate byte size of the corresponding utf-8 codepoint.
    
    auto decrement_cursor = [](UIImmediateElement &element, int32 size) {
      element.widget.input_cursor_position.x -= size;
      if (element.widget.input_cursor_position.x < 0) {
        element.widget.input_cursor_position.x = 0;
      }
      element.animation.focused_time_offset = Time::GetTime();
    };
    auto increment_cursor = [](UIImmediateElement &element, int32 size, const String &text) {
      element.widget.input_cursor_position.x += size;
      if (element.widget.input_cursor_position.x > static_cast<int32>(text.size())) {
        element.widget.input_cursor_position.x = static_cast<int32>(text.size());
      }
      element.animation.focused_time_offset = Time::GetTime();
    };
    
    UIImmediateInteraction interaction = InteractWithElement(element);
    if (interaction.focused) {
      for (AppEvent *app_event : Input::GetEvents()) {
        AppEventDispatcher dispatcher = AppEventDispatcher(*app_event);
      
        dispatcher.Dispatch<KeyPressedAppEvent>([&element, &decrement_cursor, &increment_cursor, &text, &interaction](KeyPressedAppEvent &pressed_event) {
          KeyCode key_code = pressed_event.GetKeyCode();
          if (key_code == KeyCode::Return) {
            interaction.input_submitted = true;
            
            // We lose focus on return.
            s_state.focused_element = 0;
          } else if (key_code == KeyCode::Left) {
            decrement_cursor(element, static_cast<int32>(StringUtils::GetCodepointSizeBeforeOffsetFromUtf8(text, element.widget.input_cursor_position.x)));
          } else if (key_code == KeyCode::Right) {
            increment_cursor(element, static_cast<int32>(StringUtils::GetCodepointSizeFromUtf8(text, element.widget.input_cursor_position.x)), text);
          } else if (key_code == KeyCode::Home) {
            element.widget.input_cursor_position = Vector2Int();
          } else if (key_code == KeyCode::End) {
            element.widget.input_cursor_position = Vector2Int(static_cast<int32>(element.widget.text.size()), 0); 
          } else if (key_code == KeyCode::Back) {
            if (!text.empty()) {
              uint32 codepoint_size = StringUtils::GetCodepointSizeBeforeOffsetFromUtf8(text, element.widget.input_cursor_position.x);
              int32 erase_position = element.widget.input_cursor_position.x - static_cast<int32>(codepoint_size);
              if (erase_position >= 0) {
                text.erase(erase_position, codepoint_size);
                decrement_cursor(element, static_cast<int32>(codepoint_size));
                interaction.input_changed = true;
              }
            }
          } else if (key_code == KeyCode::Delete) {
            if (!text.empty()) {
              uint32 codepoint_size = StringUtils::GetCodepointSizeFromUtf8(text, element.widget.input_cursor_position.x);
              int32 erase_position = element.widget.input_cursor_position.x;
              if (erase_position >= 0 && erase_position < static_cast<int32>(text.size())) {
                text.erase(erase_position, codepoint_size);
                interaction.input_changed = true;
              }  
            }
          }

          if (pressed_event.HasKeyModifier(KeyModifier::Control)) {
            if (key_code == KeyCode::V) {
              String clipboard = Input::GetClipboard();
              text.insert(element.widget.input_cursor_position.x, clipboard);
              increment_cursor(element, static_cast<int32>(clipboard.size()), text);
              interaction.input_changed = true;
            }
          }
        });

        dispatcher.Dispatch<KeyTypedAppEvent>([&element, &increment_cursor, theme, &interaction, &text](KeyTypedAppEvent &typed_event) {
          String key_typed = typed_event.GetCharacter();
          bool8 has_characters = true;
          Array<uint32> codepoints = StringUtils::GetCodepointsFromUtf8(key_typed);
          for (uint32 codepoint : codepoints) {
            if (codepoint == ' ' || codepoint == '\t' || codepoint == '\n') {
              continue;
            }
            
            if (!theme->font->HasCodepoint(codepoint) || codepoint == '\r') {
              has_characters = false;
              break;
            }
          }

          if (has_characters) {
            text.insert(element.widget.input_cursor_position.x, key_typed);
            increment_cursor(element, static_cast<int32>(key_typed.size()), text);
            interaction.input_changed = true;
          }
        });
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
    {
      HYP_PROFILE_SCOPE("UIImmediate.Layout.Step1")
      
      IterateHierarchyForLayout(s_state.root_element, [](UIImmediateElement &element) {
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
      });
    }
    
    // Step 2: Calculate all upwards dependent sizes like percentage of parent and prepare fill.
    {
      HYP_PROFILE_SCOPE("UIImmediate.Layout.Step2")
      
      IterateHierarchyForLayout(s_state.root_element, [](UIImmediateElement &element) {
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
      });
    }

    // Step 3: Calculate fill.
    {
      HYP_PROFILE_SCOPE("UIImmediate.Layout.Step3")
      
      IterateHierarchyForLayout(s_state.root_element, [](UIImmediateElement &element) {
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
      });
    }
    
    // Last step: Calculate relative and absolute position as well as the final rect for rendering and scissor.
    {
      HYP_PROFILE_SCOPE("UIImmediate.Layout.Step4")
      
      IterateHierarchyForLayout(s_state.root_element, [](UIImmediateElement &element) {
        UIImmediateElement *parent = element.hierarchy.parent;

        bool8 is_empty = (element.widget.flags & UIImmediateWidgetFlags::Empty) == UIImmediateWidgetFlags::Empty;
        bool8 is_scrollable = (element.widget.flags & UIImmediateWidgetFlags::Scrollable) == UIImmediateWidgetFlags::Scrollable;
        bool8 is_text = (element.widget.flags & UIImmediateWidgetFlags::Text) == UIImmediateWidgetFlags::Text;
        bool8 is_button = (element.widget.flags & UIImmediateWidgetFlags::Button) == UIImmediateWidgetFlags::Button;
        bool8 is_input = (element.widget.flags & UIImmediateWidgetFlags::Input) == UIImmediateWidgetFlags::Input;
        
        // Position ourself relative to our previous sibling depending on the layout axis of our parent.
        float32 relative_position[2] = { };
        if (parent != nullptr && !is_empty) {
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
          
          // Add scroll offset.
          bool8 parent_is_scrollable = (parent->widget.flags & UIImmediateWidgetFlags::Scrollable) == UIImmediateWidgetFlags::Scrollable;
          if (parent_is_scrollable) {
            relative_position[1] -= parent->widget.scroll_offset;
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

        // Set persistent layout state.
        element.layout.rect = Rect(rect_position, rect_size);
        element.layout.child_size[0] = element.layout.computed_child_size_sum[0];
        element.layout.child_size[1] = element.layout.computed_child_size_sum[1];

        // Figure our our scissor rect.
        // NOTE: The scissor rect is still in ui-space-coordinates not screen-space coordinate as needed for actual rendering.
        if (is_text || is_input || is_button || is_scrollable || parent == nullptr) {
          // When text or a scrollable is involved we use ourselves as the scissor rect.
          element.layout.scissor_rect = {
            static_cast<int32>(element.layout.rect.x),
            static_cast<int32>(element.layout.rect.y),
            static_cast<int32>(element.layout.rect.width),
            static_cast<int32>(element.layout.rect.height)  
          };
        } else if (parent != nullptr) {
          // In all other cases we inherit the scissor rect from our parent.
          element.layout.scissor_rect = parent->layout.scissor_rect;
        }

        // Clip our scissor rect to our parents.
        if (parent != nullptr) {
          int32 width = element.layout.scissor_rect.x + element.layout.scissor_rect.width;
          int32 height = element.layout.scissor_rect.y + element.layout.scissor_rect.height;
          int32 parent_width = parent->layout.scissor_rect.x + parent->layout.scissor_rect.width;
          int32 parent_height = parent->layout.scissor_rect.y + parent->layout.scissor_rect.height;
          if (width > parent_width) {
            element.layout.scissor_rect.width -= width - parent_width;
          }
          if (height > parent_height) {
            element.layout.scissor_rect.height -= height - parent_height;
          }
        }
      });
    }
  }

  //--------------------------------------------------------------
  void UIImmediate::Render() {
    HYP_PROFILE_SCOPE("UIImmediate::Render")
    
    IterateHierarchyForRender(s_state.root_element, [](UIImmediateElement &element) {
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

      Rect rect = element.layout.rect;
      // Transform scissor rect to screen-space coordinates.
      RectInt scissor_rect = element.layout.scissor_rect;
      Vector2 screen_space_point = UISpacePointToScreenPoint(Vector2(static_cast<float32>(scissor_rect.x), static_cast<float32>(scissor_rect.y)));
      scissor_rect.x = static_cast<int32>(screen_space_point.x);
      scissor_rect.y = static_cast<int32>(screen_space_point.y);
      
      if (is_separator || is_panel || is_button || is_toggle || is_input || is_image) {
        Color color = GetBackgroundColor(element);

        DrawRect(rect, color);
        if (is_image) {
          Flush(scissor_rect, AssetManager::GetMaterialPrimitive(MaterialPrimitive::UI), element.widget.texture, false);
        } else {
          Flush(scissor_rect);
        }
      }

      if (is_text || is_button || is_toggle || is_input) {
        Vector2 text_offset;
        // Figure out wrapping offset.
        if (is_input && s_state.focused_element == element.id.id) {
          uint32 codepoint_offset = StringUtils::GetCodepointOffsetFromUtf8(element.widget.text, element.widget.input_cursor_position.x);
          
          Array<uint32> codepoints = StringUtils::GetCodepointsFromUtf8(element.widget.text);
          Vector2 cursor_position = TextUtilities::GetCursorPosition(
            theme.font,
            codepoints,
            1.0f,
            element.widget.text_alignment,
            rect,
            Vector2Int(static_cast<int32>(codepoint_offset), 0)
          );

          if (cursor_position.x > (rect.x + rect.width)) {
            float32 offset = ((rect.x + rect.width) - cursor_position.x) - theme.input_cursor_width;
            text_offset.x = offset;
          }
        }
        
        if (theme.text_shadow_enabled) {
          Rect shadow_rect = rect;
          shadow_rect.position += theme.text_shadow_offset;
          DrawText(shadow_rect, element.widget.text, theme.font, element.widget.text_alignment, theme.text_shadow_color, text_offset);  
        }

        Color color = GetTextColor(element);
        DrawText(rect, element.widget.text, theme.font, element.widget.text_alignment, color, text_offset);
        
        Flush(scissor_rect, AssetManager::GetMaterialPrimitive(MaterialPrimitive::Font), theme.font->GetTexture());
      }

      // Render cursor for input field when focused.
      if (is_input && s_state.focused_element == element.id.id) {
        if (Time::BetweenInterval(theme.input_cursor_blink_rate, element.animation.focused_time_offset - theme.input_cursor_blink_rate)) {
          // We need to convert the cursor position from a byte offset to a utf-8 codepoint offset.
          uint32 codepoint_offset = StringUtils::GetCodepointOffsetFromUtf8(element.widget.text, element.widget.input_cursor_position.x);
          
          Array<uint32> codepoints = StringUtils::GetCodepointsFromUtf8(element.widget.text);
          Vector2 cursor_position = TextUtilities::GetCursorPosition(
            theme.font,
            codepoints,
            1.0f,
            element.widget.text_alignment,
            rect,
            Vector2Int(static_cast<int32>(codepoint_offset), 0)
          );

          // Add wrapping offset.
          if (cursor_position.x > (rect.x + rect.width)) {
            float32 offset = ((rect.x + rect.width) - cursor_position.x) - theme.input_cursor_width;
            cursor_position.x += offset; 
          }
          
          Rect cursor_rect = {
            cursor_position.x,
            cursor_position.y,
            theme.input_cursor_width,
            static_cast<float32>(theme.font->GetSize())
          };
          
          DrawRect(cursor_rect, theme.input_cursor_color);
          Flush(scissor_rect);
        }
      }
      
      return true;
    });
    Flush(s_state.root_element.layout.scissor_rect);
    
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
  void UIImmediate::DrawText(Rect rect, const String &text, Font *font, UI::TextAlignment alignment, Color color, Vector2 offset) {
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
    generation_settings.offset = offset;

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
      return parent_rect.Overlaps(element_rect);
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
      HYP_LOG_ERROR("UI", "Elements share the id '{}' - This leads to undefined behaviour!", id_text);
      HYP_DEBUG_BREAK;
    }
    element->id.looked_up_this_frame = true;
    element->id.last_frame_touched_index = s_state.current_frame_index;

    // We have to remember to reset non persistent state.
    element->hierarchy = { };
    // The layout contains both persistent and non persistent state.
    Rect rect = element->layout.rect;
    RectInt scissor_rect = element->layout.scissor_rect;
    float32 child_sum[2] = { element->layout.child_size[0], element->layout.child_size[1] };
    element->layout = { };
    element->layout.rect = rect;
    element->layout.scissor_rect = scissor_rect;
    element->layout.child_size[0] = child_sum[0];
    element->layout.child_size[1] = child_sum[1];

    // HACK: This is a weird workaround but prevents things from being permanently stuck in an invalid scroll offset.
    if (Display::HasChangedSize()) {
      element->widget.scroll_offset = 0.0f;
    }
    
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
    UIImmediateInteraction result = { };
    result.element = &element;

    if ((element.widget.flags & UIImmediateWidgetFlags::Scrollable) == UIImmediateWidgetFlags::Scrollable) {
      if (IsInsideRect(element.layout.rect, s_state.mouse_position)) {
        // Check if we have an actual overflow.
        if (element.layout.child_size[1] > element.layout.rect.height) {
          // Overflow is negative because positive y axis goes down.
          float32 overflow = -(element.layout.child_size[1] - element.layout.rect.height);
          element.widget.scroll_offset += s_state.mouse_scroll * element.widget.theme->scroll_multiplier;
          element.widget.scroll_offset = Math::Clamp(element.widget.scroll_offset, overflow, 0.0f);
        }
      }
    }
    
    if (!IsInsideParent(element)) {
      return result;
    }

    if ((element.widget.flags & UIImmediateWidgetFlags::Interactable) == UIImmediateWidgetFlags::Interactable) {
      UIImmediateId id = element.id.id;
      
      bool8 is_inside = IsInsideRect(element.layout.rect, s_state.mouse_position);
      bool8 is_inside_scissor = element.layout.scissor_rect.Contains(
        Vector2Int(static_cast<int32>(s_state.mouse_position.x), static_cast<int32>(s_state.mouse_position.y))
      );
      
      if (is_inside && is_inside_scissor) {
        s_state.hovered_element = id;
        if (s_state.pressed_element == 0 && (s_state.is_left_mouse_down || s_state.is_right_mouse_down)) {
          s_state.pressed_element = id;
          if ((element.widget.flags & UIImmediateWidgetFlags::Focusable) == UIImmediateWidgetFlags::Focusable) {
            s_state.focused_element = id;

            // We do a check to see that we did not have focus before to reset some state.
            if (s_state.last_focused_element != id) {
              element.animation.focused_time_offset = Time::GetTime();
              
              // Position the cursor at the end for an input field.
              element.widget.input_cursor_position = Vector2Int(static_cast<int32>(element.widget.text.size()), 0);
            }
          }
        }
      }

      bool8 is_hovered_widget = s_state.hovered_element == id;
      bool8 is_pressed_widget = s_state.pressed_element == id;
      bool8 is_focused_widget = s_state.focused_element == id;

      result.hovered = is_hovered_widget;
      result.focused = is_focused_widget;
      result.clicked = s_state.is_left_mouse_up && is_hovered_widget && is_pressed_widget;
      result.right_clicked = s_state.is_right_mouse_up && is_hovered_widget && is_pressed_widget;
      
      return result;
    }

    return result;
  }
  
  //--------------------------------------------------------------
  void UIImmediate::IterateHierarchyForLayout(UIImmediateElement &element, const std::function<void(UIImmediateElement &)> &callback) {
    callback(element);
    UIImmediateElement *child = element.hierarchy.first_child;
    for (uint64 i = 0; i < element.hierarchy.child_count; ++i) {
      IterateHierarchyForLayout(*child, callback);
      child = child->hierarchy.next_sibling;
    }
  }

  //--------------------------------------------------------------
  void UIImmediate::IterateHierarchyForRender(UIImmediateElement &element, const std::function<bool8(UIImmediateElement &)> &callback) {
    if (callback(element)) {
      UIImmediateElement *child = element.hierarchy.first_child;
      for (uint64 i = 0; i < element.hierarchy.child_count; ++i) {
        IterateHierarchyForRender(*child, callback);
        child = child->hierarchy.next_sibling;
      }  
    }
  }

}
 