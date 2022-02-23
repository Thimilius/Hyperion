//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_element.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/core/app/display.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  //--------------------------------------------------------------
  void UIElementStyle::SetColor(Color color) {
    m_color = color;
    m_element->MarkDirty();
  }

  //--------------------------------------------------------------
  void UIElementStyle::SetOpacity(float32 opacity) {
    m_opacity = opacity;
    m_element->MarkDirty();
  }

  //--------------------------------------------------------------
  void UIElementStyle::SetColorBlock(const ColorBlock &color_block) {
    m_color_block = color_block;
    m_element->MarkDirty();
  }

  //--------------------------------------------------------------
  void UIElementStyle::SetShadow(const Shadow &shadow) {
    m_shadow = shadow;
    m_element->MarkDirty();
  }

  // FIXME: Check for hierarchy cycles.

  //--------------------------------------------------------------
  void UIElementHierarchy::SetParent(UIElement *parent) {
    if (parent != m_parent) {
      if (m_parent) {
        m_parent->m_hierarchy.m_children.Remove(m_element);
      }

      m_parent = parent;

      if (parent) {
        parent->m_hierarchy.m_children.Add(m_element);
        parent->MarkHierarchyDirty();
      }
    }
  }

  //--------------------------------------------------------------
  void UIElementHierarchy::AddChild(UIElement *child) {
    if (child && !m_children.Contains(child)) {
      m_children.Add(child);
      if (child->m_hierarchy.m_parent != nullptr) {
        child->m_hierarchy.m_parent->m_hierarchy.m_children.Remove(child);
      }
      child->m_hierarchy.m_parent = m_element;

      m_element->MarkHierarchyDirty();
    }
  }

  //--------------------------------------------------------------
  void UIElementHierarchy::RemoveChildren() {
    // TODO: We need a properly defined way to "destroy" ui elements.

    for (UIElement *child : m_children) {
      delete child;
    }
    m_children.Clear();
  }

  //--------------------------------------------------------------
  UIElement::UIElement() {
    m_style.m_element = this;
    m_hierarchy.m_element = this;
  }

  //--------------------------------------------------------------
  void UIElement::MarkHierarchyDirty() {
    MarkDirty();

    for (UIElement *child : m_hierarchy.m_children) {
      child->MarkHierarchyDirty();
    }
  }

  //--------------------------------------------------------------
  void UIElement::SetInteractable(bool8 is_interactable) {
    m_state.is_interactable = is_interactable;

    if (!m_state.is_interactable) {
      GetRenderer().color = m_style.m_color_block.disabled_color;
    } else {
      GetRenderer().color = m_style.m_color_block.normal_color;
    }
  }

  //--------------------------------------------------------------
  void UIElement::SetPosition(Vector2 position) {
    m_local_position = position;

    MarkHierarchyDirty();
  }

  //--------------------------------------------------------------
  void UIElement::SetRotation(float32 rotation) {
    m_local_rotation = rotation;

    MarkHierarchyDirty();
  }

  //--------------------------------------------------------------
  void UIElement::SetScale(Vector2 scale) {
    m_local_scale = scale;

    MarkHierarchyDirty();
  }

  //--------------------------------------------------------------
  Rect UIElement::GetWorldRect() const {
    Vector3 world_corners[4];
    GetWorldCorners(world_corners);
    return Rect(Vector2(world_corners[2]), Vector2(world_corners[0] - world_corners[2]));
  }

  //--------------------------------------------------------------
  void UIElement::SetSize(Vector2 size) {
    m_size = size;

    MarkHierarchyDirty();
  }

  //--------------------------------------------------------------
  void UIElement::SetPivot(Vector2 pivot) {
    m_pivot = pivot;

    MarkHierarchyDirty();
  }

  //--------------------------------------------------------------
  void UIElement::SetAnchorMin(Vector2 anchor_min) {
    m_anchor_min = anchor_min;

    MarkHierarchyDirty();
  }

  //--------------------------------------------------------------
  void UIElement::SetAnchorMax(Vector2 anchor_max) {
    m_anchor_max = anchor_max;

    MarkHierarchyDirty();
  }

  //--------------------------------------------------------------
  void UIElement::SetAnchorOffsetMin(Vector2 anchor_offset_min) {
    m_anchor_offset_min = anchor_offset_min;

    MarkHierarchyDirty();
  }

  //--------------------------------------------------------------
  void UIElement::SetAnchorOffsetMax(Vector2 anchor_offset_max) {
    m_anchor_offset_max = anchor_offset_max;

    MarkHierarchyDirty();
  }

  //--------------------------------------------------------------
  void UIElement::GetLocalCorners(Vector3 corners[4]) const {
    HYP_ASSERT(corners);

    Vector2 min = m_rect.GetMin();
    Vector2 max = m_rect.GetMax();

    corners[0] = Vector3(max.x, max.y, 0.0f);
    corners[1] = Vector3(max.x, min.y, 0.0f);
    corners[2] = Vector3(min.x, min.y, 0.0f);
    corners[3] = Vector3(min.x, max.y, 0.0f);
  }

  //--------------------------------------------------------------
  void UIElement::GetWorldCorners(Vector3 corners[4]) const {
    HYP_ASSERT(corners);

    GetLocalCorners(corners);
    TransformCorners(corners);
  }

  //--------------------------------------------------------------
  void UIElement::TransformCorners(Vector3 corners[4]) const {
    HYP_ASSERT(corners);

    for (uint64 i = 0; i < 4; i++) {
      corners[i] = m_transform * corners[i];
    }
  }

  //--------------------------------------------------------------
  void UIElement::SetAnchorPreset(AnchorPreset anchor_preset) {
    switch (anchor_preset) {
      case AnchorPreset::TopLeft:
      {
        m_anchor_min = Vector2(0.0f, 1.0f);
        m_anchor_max = Vector2(0.0f, 1.0f);
        m_pivot = Vector2(0.0f, 1.0f);
        break;
      }
      case AnchorPreset::TopCenter:
      {
        m_anchor_min = Vector2(0.5f, 1.0f);
        m_anchor_max = Vector2(0.5f, 1.0f);
        m_pivot = Vector2(0.5f, 1.0f);
        break;
      }
      case AnchorPreset::TopRight:
      {
        m_anchor_min = Vector2(1.0f, 1.0f);
        m_anchor_max = Vector2(1.0f, 1.0f);
        m_pivot = Vector2(1.0f, 1.0f);
        break;
      }
      case AnchorPreset::MiddleLeft:
      {
        m_anchor_min = Vector2(0.0f, 0.5f);
        m_anchor_max = Vector2(0.0f, 0.5f);
        m_pivot = Vector2(0.0f, 0.5f);
        break;
      }
      case AnchorPreset::MiddleCenter:
      {
        m_anchor_min = Vector2(0.5f, 0.5f);
        m_anchor_max = Vector2(0.5f, 0.5f);
        m_pivot = Vector2(0.5f, 0.5f);
        break;
      }
      case AnchorPreset::MiddleRight:
      {
        m_anchor_min = Vector2(1.0f, 0.5f);
        m_anchor_max = Vector2(1.0f, 0.5f);
        m_pivot = Vector2(1.0f, 0.5f);
        break;
      }
      case AnchorPreset::BottomLeft:
      {
        m_anchor_min = Vector2(0.0f, 0.0f);
        m_anchor_max = Vector2(0.0f, 0.0f);
        m_pivot = Vector2(0.0f, 0.0f);
        break;
      }
      case AnchorPreset::BottomCenter:
      {
        m_anchor_min = Vector2(0.5f, 0.0f);
        m_anchor_max = Vector2(0.5f, 0.0f);
        m_pivot = Vector2(0.5f, 0.0f);
        break;
      }
      case AnchorPreset::BottomRight:
      {
        m_anchor_min = Vector2(1.0f, 0.0f);
        m_anchor_max = Vector2(1.0f, 0.0f);
        m_pivot = Vector2(1.0f, 0.0f);
        break;
      }
      case AnchorPreset::TopStretchHorizontal:
      {
        m_anchor_min = Vector2(0.0f, 1.0f);
        m_anchor_max = Vector2(1.0f, 1.0f);
        m_pivot = Vector2(0.5f, 1.0f);
        m_size.x = 0.0f;
        break;
      }
      case AnchorPreset::MiddleStretchHorizontal:
      {
        m_anchor_min = Vector2(0.0f, 0.5f);
        m_anchor_max = Vector2(1.0f, 0.5f);
        m_pivot = Vector2(0.5f, 0.5f);
        m_size.x = 0.0f;
        break;
      }
      case AnchorPreset::BottomStretchHorizontal:
      {
        m_anchor_min = Vector2(0.0f, 0.0f);
        m_anchor_max = Vector2(1.0f, 0.0f);
        m_pivot = Vector2(0.5f, 0.0f);
        m_size.x = 0.0f;
        break;
      }
      case AnchorPreset::LeftStretchVertical:
      {
        m_anchor_min = Vector2(0.0f, 0.0f);
        m_anchor_max = Vector2(0.0f, 1.0f);
        m_pivot = Vector2(0.0f, 0.5f);
        m_size.y = 0.0f;
        break;
      }
      case AnchorPreset::CenterStretchVertical:
      {
        m_anchor_min = Vector2(0.5f, 0.0f);
        m_anchor_max = Vector2(0.5f, 1.0f);
        m_pivot = Vector2(0.5f, 0.5f);
        m_size.y = 0.0f;
        break;
      }
      case AnchorPreset::RightStretchVertical:
      {
        m_anchor_min = Vector2(1.0f, 0.0f);
        m_anchor_max = Vector2(1.0f, 1.0f);
        m_pivot = Vector2(1.0f, 0.5f);
        m_size.y = 0.0f;
        break;
      }
      case AnchorPreset::StretchAll:
      {
        m_anchor_min = Vector2(0.0f, 0.0f);
        m_anchor_max = Vector2(1.0f, 1.0f);
        m_pivot = Vector2(0.5f, 0.5f);
        m_size = Vector2(0.0f, 0.0f);
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
    }

    MarkHierarchyDirty();
  }

  //--------------------------------------------------------------
  void UIElement::OnEvent(UIEvent &event) {
    UIEventType event_type = event.GetType();

    if (event_type == UIEventType::PointerDown) {
      m_state.is_pressed = true;
    } else if (event_type == UIEventType::PointerUp) {
      m_state.is_pressed = false;
    } else if (event_type == UIEventType::PointerEnter) {
      m_state.is_highlighted = true;
    } else if (event_type == UIEventType::PointerExit) {
      m_state.is_highlighted = false;
    } else if (event_type == UIEventType::Select) {
      m_state.is_selected = true;
    } else if (event_type == UIEventType::Deselect) {
      m_state.is_selected = false;
    }

    DoStateTransition(GetSelectionState());
  }

  //--------------------------------------------------------------
  bool8 UIElement::ContainsScreenPoint(Vector2 screen_point) {
    Vector2 ui_space_point = ScreenPointToUISpacePoint(screen_point);

    auto is_left = [](Vector2 p0, Vector2 p1, Vector2 p2) {
      return ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y));
    };

    Vector3 world_corners[4];
    GetWorldCorners(world_corners);
    Vector2 p1 = world_corners[0];
    Vector2 p2 = world_corners[1];
    Vector2 p3 = world_corners[2];
    Vector2 p4 = world_corners[3];

    // NOTE: Counter clockwise order of points is important.
    return (is_left(p1, p4, ui_space_point) >= 0.0f && is_left(p4, p3, ui_space_point) >= 0.0f && is_left(p3, p2, ui_space_point) >= 0.0f && is_left(p2, p1, ui_space_point) >= 0.0f);
  }

  //--------------------------------------------------------------
  void UIElement::Rebuild(float32 ui_scale, MeshBuilder &mesh_builder) {
    m_is_dirty = false;

    RecalculateTransform(ui_scale);

    OnRebuildLayout();

    mesh_builder.Clear();
    Shadow shadow = GetStyle().GetShadow();
    if (shadow.enabled) {
      Color shadow_color = shadow.color;
      shadow_color.a *= GetStyle().GetOpacity();

      if (shadow_color.a > 0.0f) {
        OnRebuildShadowGeometry(mesh_builder, shadow_color, shadow.offset);
      }
    }
    OnRebuildGeometry(mesh_builder);
    AssetManager::Unload(m_renderer.mesh);
    m_renderer.mesh = mesh_builder.CreateMesh();
  }

  //--------------------------------------------------------------
  Vector2 UIElement::ScreenPointToUISpacePoint(Vector2 screen_point) {
    float32 display_width = static_cast<float32>(Display::GetWidth());
    float32 display_height = static_cast<float32>(Display::GetHeight());
    screen_point.x -= display_width / 2.0f;
    screen_point.y -= display_height / 2.0f;
    return screen_point;
  }

  //--------------------------------------------------------------
  SelectionState UIElement::GetSelectionState() const {
    if (!m_state.is_interactable) {
      return SelectionState::Disabled;
    } else if (m_state.is_pressed) {
      return SelectionState::Pressed;
    } else if (m_state.is_selected) {
      return SelectionState::Selected;
    } else if (m_state.is_highlighted) {
      return SelectionState::Highlighted;
    } else {
      return SelectionState::Normal;
    }
  }

  //--------------------------------------------------------------
  void UIElement::DoStateTransition(SelectionState state) {
    switch (state) {
      case SelectionState::Highlighted: GetRenderer().color = m_style.m_color_block.highlighted_color; break;
      case SelectionState::Pressed: GetRenderer().color = m_style.m_color_block.pressed_color; break;
      case SelectionState::Selected: GetRenderer().color = m_style.m_color_block.selected_color; break;
      case SelectionState::Disabled: GetRenderer().color = m_style.m_color_block.disabled_color; break;
      case SelectionState::Normal:
      default: GetRenderer().color = m_style.m_color_block.normal_color; break;
    }
  }

  //--------------------------------------------------------------
  void UIElement::OnRebuildLayout() {
    if (m_layout.layout_type == LayoutType::None) {
      return;
    }

    // We first have to gather all relevant dimensions and sizes.
    Vector2 layout_size = Vector2();
    for (UIElement *child : m_hierarchy.m_children) {
      if (child->m_layout.ignore_layout) {
        continue;
      }

      Vector2 size = child->GetSize();
      switch (m_layout.layout_type) {
        case LayoutType::Horizontal:
        {
          layout_size.x += size.x + m_layout.spacing.x;
          if (size.y > layout_size.y) {
            layout_size.y = size.y;
          }
          break;
        }
        case LayoutType::Vertical:
        {
          if (size.x > layout_size.x) {
            layout_size.x = size.x;
          }
          layout_size.y += size.y + m_layout.spacing.y;
          break;
        }
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
      }

      child->SetAnchorPreset(AnchorPreset::BottomLeft);
    }
    if (!m_hierarchy.m_children.IsEmpty()) {
      switch (m_layout.layout_type) {
        case LayoutType::Horizontal:
        {
          layout_size.x -= m_layout.spacing.x;
          break;
        }
        case LayoutType::Vertical:
        {
          layout_size.y -= m_layout.spacing.y;
          break;
        }
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
      }
    }

    Rect rect = GetLocalRect();
    Vector2 offset = GetLayoutPosition(m_layout.child_alignment, Vector2(rect.width, rect.height), layout_size);
    for (UIElement *child : m_hierarchy.m_children) {
      if (child->m_layout.ignore_layout) {
        continue;
      }

      child->SetPosition(offset);

      Vector2 size = child->GetSize();
      switch (m_layout.layout_type) {
        case LayoutType::Horizontal:
        {
          offset.x += size.x + m_layout.spacing.x;
          break;
        }
        case LayoutType::Vertical:
        {
          offset.y += size.y + m_layout.spacing.y;
          break;
        }
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
      }
    }
  }

  //--------------------------------------------------------------
  void UIElement::OnRebuildShadowGeometry(MeshBuilder &mesh_builder, Color shadow_color, Vector2 shadow_offset) {
    Vector3 corners[4];
    GetLocalCorners(corners);
    for (uint64 i = 0; i < 4; i++) {
      corners[i] += Vector3(shadow_offset, 0.0f);
    }

    AddQuad(mesh_builder, corners, shadow_color);
  }

  //--------------------------------------------------------------
  void UIElement::OnRebuildGeometry(MeshBuilder &mesh_builder) {
    Vector3 corners[4];
    GetLocalCorners(corners);

    AddQuad(mesh_builder, corners, m_style.m_color);
  }

  //--------------------------------------------------------------
  void UIElement::AddQuad(MeshBuilder &mesh_builder, Vector3 corners[4], Color color) {
    HYP_ASSERT(corners);

    TransformCorners(corners);

    mesh_builder.AddQuad(corners, color);
  }

  //--------------------------------------------------------------
  void UIElement::RecalculateTransform(float32 ui_scale) {
    Vector2 size = m_size;
    UIElement *parent = m_hierarchy.GetParent();

    Vector3 final_local_position;
    {
      float32 inverse_ui_scale = 1.0f / ui_scale;
      Vector2 parent_pivot = parent ? parent->m_pivot : Vector2(0.5f, 0.5f);
      Vector2 parent_size = parent ? parent->m_rect.size : Vector2(inverse_ui_scale * Display::GetWidth(), inverse_ui_scale * Display::GetHeight());
      Vector2 half_parent_size = parent_size / 2.0f;

      float32 anchor_x = m_anchor_max.x - m_anchor_min.x;
      float32 anchor_y = m_anchor_max.y - m_anchor_min.y;
      float32 anchor_x_size = anchor_x * parent_size.x;
      float32 anchor_y_size = anchor_y * parent_size.y;
      size.x += anchor_x * parent_size.x;
      size.y += anchor_y * parent_size.y;
      size.x -= m_anchor_offset_min.x + m_anchor_offset_max.x;
      size.y -= m_anchor_offset_min.y + m_anchor_offset_max.y;
      m_rect = Rect(-m_pivot * size, size);

      // First we have our local position which acts as a simple offset.
      final_local_position = Vector3(m_local_position, 0.0f);
      // We have to take into account the parent pivot which has an effect on our local position.
      final_local_position.x += (1.0f - parent_pivot.x) * parent_size.x - half_parent_size.x;
      final_local_position.y += (1.0f - parent_pivot.y) * parent_size.y - half_parent_size.y;
      // Now we need to take into account our own pivot.
      final_local_position.x += m_anchor_min.x * parent_size.x - half_parent_size.x;
      final_local_position.y += m_anchor_min.y * parent_size.y - half_parent_size.y;
      // Add the anchoring.
      final_local_position.x += m_pivot.x * anchor_x_size;
      final_local_position.y += m_pivot.y * anchor_y_size;
      // And finally the anchoring offset.
      final_local_position.x += ((1.0f - m_pivot.x) * m_anchor_offset_min.x) - ((1.0f - m_pivot.x) * m_anchor_offset_max.x);
      final_local_position.y += ((1.0f - m_pivot.y) * m_anchor_offset_min.y) - ((1.0f - m_pivot.y) * m_anchor_offset_max.y);
    }

    {
      if (parent == nullptr) {
        m_derived_position = ui_scale * final_local_position;
        m_derived_rotation = Quaternion::FromAxisAngle(Vector3::Forward(), m_local_rotation);
        m_derived_scale = Vector3(ui_scale * m_local_scale, 1.0f);
      } else {
        m_derived_position = parent->m_derived_rotation * (parent->m_derived_scale * final_local_position);
        m_derived_position += parent->m_derived_position;
        m_derived_rotation = parent->m_derived_rotation * Quaternion::FromAxisAngle(Vector3::Forward(), m_local_rotation);
        m_derived_scale = parent->m_derived_scale * Vector3(m_local_scale, 1.0f);
      }
      m_transform = Matrix4x4::TRS(m_derived_position, m_derived_rotation, m_derived_scale);
    }
  }

  //--------------------------------------------------------------
  Vector2 UIElement::GetLayoutPosition(ChildAlignment child_alignment, Vector2 container_layout, Vector2 layout_size) {
    Vector2 position = Vector2();

    switch (child_alignment) {
      case ChildAlignment::TopLeft:
      {
        position = Vector2(0.0f, container_layout.y - layout_size.y);
        break;
      }
      case ChildAlignment::TopCenter:
      {
        position = Vector2((container_layout.x / 2.0f) - (layout_size.x / 2.0f), container_layout.y - layout_size.y);
        break;
      }
      case ChildAlignment::TopRight:
      {
        position = Vector2(container_layout.x - layout_size.x, container_layout.y - layout_size.y);
        break;
      }
      case ChildAlignment::MiddleLeft:
      {
        position = Vector2(0.0f, (container_layout.y / 2.0f) - (layout_size.y / 2.0f));
        break;
      }
      case ChildAlignment::MiddleCenter:
      {
        position = Vector2((container_layout.x / 2.0f) - (layout_size.x / 2.0f), (container_layout.y / 2.0f) - (layout_size.y / 2.0f));
        break;
      }
      case ChildAlignment::MiddleRight:
      {
        position = Vector2(container_layout.x - layout_size.x, (container_layout.y / 2.0f) - (layout_size.y / 2.0f));
        break;
      }
      case ChildAlignment::BottomLeft:
      {
        position = Vector2(0.0f, 0.0f);
        break;
      }
      case ChildAlignment::BottomCenter:
      {
        position = Vector2((container_layout.x / 2.0f) - (layout_size.x / 2.0f), 0.0f);
        break;
      }
      case ChildAlignment::BottomRight:
      {
        position = Vector2(container_layout.x - layout_size.x, 0.0f);
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
    }

    return Vector3(position, 0.0f);
  }

}