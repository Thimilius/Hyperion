//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/core/app/events/app_event.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/ui/ui_event.hpp"
#include "hyperion/ui/ui_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  struct UIImmediateElement;
  
  using UIImmediateId = uint64;

  struct UIImmediateMesh {
    uint32 vertex_count = 0;
    Mesh *mesh = nullptr;
  };
  
  struct UIImmediateMeshDraw {
    UIImmediateMesh mesh;
    Material *material = nullptr;
    Texture *texture = nullptr;

    Color color = Color::White();

    RectInt scissor;
    bool8 enable_blending = false;
    
    uint32 render_texture_attachment_index = 0;
  };

  struct UIImmediateInteraction {
    UIImmediateElement *element;
    
    bool8 hovered = false;
    bool8 focused = false;
    bool8 clicked = false;
    bool8 right_clicked = false;

    bool8 input_submitted = false;
    bool8 input_changed = false;
  };

  struct UIImmediateTheme {
    Color separator_color = Color::White();
      
    Color panel_color = Color::Grey();
    Color panel_color_hovered = Color::Grey();
    Color panel_color_pressed = Color::Grey();

    Color text_color = Color::White();
    Color text_color_hovered = Color::White();
    Color text_color_pressed = Color::White();
    
    Color button_color = Color::Grey();
    Color button_color_hovered = Color::Grey();
    Color button_color_pressed = Color::Grey();

    Color toggle_normal_color = Color::Grey();
    Color toggle_normal_color_hovered = Color::Grey();
    Color toggle_normal_color_pressed = Color::Grey();
    Color toggle_toggled_color = Color::Grey();
    Color toggle_toggled_color_hovered = Color::Grey();
    Color toggle_toggled_color_pressed = Color::Grey();
    Color toggle_toggled_text_color = Color::White();

    Color input_color = Color::Grey();
    Color input_color_hovered = Color::Grey();
    Color input_color_pressed = Color::Grey();
    Color input_color_focused = Color::Grey();
    Color input_cursor_color = Color::White();
    float32 input_cursor_width = 1.0f;
    float32 input_cursor_blink_rate = 0.75f;
    
    bool8 text_shadow_enabled = true;
    Color text_shadow_color = Color::Black();
    Vector2 text_shadow_offset = Vector2(1.0f, -1.0f);

    float32 scroll_multiplier = 10.0f;
    
    Font *font = nullptr;
  };
  
  enum class UIImmediateWidgetFlags {
    None = 0,

    Interactable = BIT(0),
    Focusable = BIT(1),
    Scrollable = BIT(2),
    
    Empty = BIT(3),
    Space = BIT(4),
    Separator = BIT(5),
    Panel = BIT(6),
    Text = BIT(7),
    Button = BIT(8),
    Toggle = BIT(9),
    Input = BIT(10),
    Image = BIT(11)
  };
  HYP_CREATE_ENUM_FLAG_OPERATORS(UIImmediateWidgetFlags)

  struct UIImmediateElement {
    struct Id {
      UIImmediateId id = 0;
      String id_text;
      uint64 last_frame_touched_index = 0;
      bool8 looked_up_this_frame = false;
    } id;
    
    struct Hierarchy {
      UIImmediateElement *parent = nullptr;

      UIImmediateElement *previous_sibling = nullptr;
      UIImmediateElement *next_sibling = nullptr;

      uint64 child_count = 0;
      UIImmediateElement *first_child = nullptr;
      UIImmediateElement *last_child = nullptr;  
    } hierarchy;

    struct Layout {
      Size semantic_size[2] = { };
      
      ChildLayout child_layout = ChildLayout::Vertical;
      float32 child_layout_offset = 0.0f;

      float32 computed_size[2] = { };
      float32 computed_relative_position[2] = { };
      float32 computed_absolute_position[2] = { };

      uint32 fill_child_count = { };
      float32 computed_child_size_sum[2] = { };

      // This is persistent state.
      Rect rect = Rect();
      float32 child_size[2] = { };
    } layout;

    struct Widget {
      UIImmediateWidgetFlags flags = UIImmediateWidgetFlags::None;

      UIImmediateTheme *theme = nullptr;
      
      float32 scroll_offset = 0.0f;
      
      String text = String();
      TextAlignment text_alignment = TextAlignment::TopLeft;
      bool8 toggled = false;
      Vector2Int input_cursor_position;
      
      Texture *texture = nullptr;
      bool8 enable_blending = true;
      uint32 render_attachment_index = 0;
    } widget;

    struct Animation {
      float32 focused_time_offset = 0.0f;
    } animation;
  };

  struct UIImmediateState {
    Vector2 mouse_position = Vector2();
    float32 mouse_scroll = 0.0f;
    bool8 is_left_mouse_down = false;
    bool8 is_left_mouse_hold = false;
    bool8 is_left_mouse_up = false;
    bool8 is_right_mouse_down = false;
    bool8 is_right_mouse_hold = false;
    bool8 is_right_mouse_up = false;
    
    UIImmediateId hovered_element = 0;
    UIImmediateId pressed_element = 0;
    UIImmediateId focused_element = 0;
    UIImmediateId last_focused_element = 0;

    uint64 current_frame_index = 0;

    UIImmediateElement root_element;
    Array<UIImmediateElement *> element_stack;
    Map<UIImmediateId, UIImmediateElement> persistent_elements;
    Map<UIImmediateId, UIImmediateElement> temporary_elements;

    Array<String> id_stack;
  };

}
