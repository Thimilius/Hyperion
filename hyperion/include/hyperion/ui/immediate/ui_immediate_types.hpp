//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/ui/ui_event.hpp"
#include "hyperion/ui/ui_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {
  
  using UIImmediateId = uint64;

  struct UIImmediateMesh {
    Mesh *mesh = nullptr;
    Material *material = nullptr;
    Texture *texture = nullptr;

    Color color = Color::White();
    bool8 enable_blending = false;
    
    uint32 render_texture_attachment_index = 0;
  };

  struct UIImmediateInteraction {
    bool8 hovered = false;
    bool8 clicked = false;
    bool8 right_clicked = false;
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
    
    bool8 text_shadow_enabled = true;
    Color text_shadow_color = Color::Black();
    Vector2 text_shadow_offset = Vector2(1.0f, -1.0f);

    Font *font = nullptr;
  };
  
  enum class UIImmediateWidgetFlags {
    None = 0,

    Interactable = BIT(0),
    
    Empty = BIT(1),
    Space = BIT(2),
    Separator = BIT(3),
    Panel = BIT(4),
    Text = BIT(5),
    Button = BIT(6),
    Toggle = BIT(7),
    Image = BIT(8)
  };
  HYP_CREATE_ENUM_FLAG_OPERATORS(UIImmediateWidgetFlags)

  struct UIImmediateElement {
    struct UIImmediateElementId {
      UIImmediateId id = 0;
      uint64 last_frame_touched_index = 0;
      bool8 looked_up_this_frame = false;
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
      Size semantic_size[2] = { };
      
      ChildLayout child_layout = ChildLayout::Vertical;
      float32 child_layout_offset = 0.0f;

      float32 computed_size[2] = { };
      float32 computed_relative_position[2] = { };
      float32 computed_absolute_position[2] = { };

      uint32 fill_child_count = { };
      float32 computed_child_size_sum[2] = { };
      
      Rect rect = Rect();
    } layout;

    struct UIImmediateElementWidget {
      UIImmediateWidgetFlags flags = UIImmediateWidgetFlags::None;

      UIImmediateTheme *theme = nullptr;
      
      String text = String();
      TextAlignment text_alignment = TextAlignment::TopLeft;

      bool8 toggled = false;

      Texture *texture = nullptr;
      bool8 enable_blending = true;
      uint32 render_attachment_index = 0;
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

    UIImmediateElement root_element;
    Array<UIImmediateElement *> element_stack;
    Map<UIImmediateId, UIImmediateElement> persistent_elements;
    Map<UIImmediateId, UIImmediateElement> temporary_elements;

    Array<String> id_stack;
  };

  struct LayoutAxes {
    uint32 fill_axis;
    uint32 leftover_axis;
  };

}
