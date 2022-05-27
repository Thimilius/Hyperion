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

  struct UIImmediateNode {
    struct UIImmediateNodeId {
      UIImmediateId id = 0;
      uint64 last_frame_touched_index = 0;
    } id;
    
    struct UIImmediateNodeHierarchy {
      UIImmediateNode *parent = nullptr;

      UIImmediateNode *previous_sibling = nullptr;
      UIImmediateNode *next_sibling = nullptr;

      uint64 child_count = 0;
      UIImmediateNode *first_child = nullptr;
      UIImmediateNode *last_child = nullptr;  
    } hierarchy;

    struct UIImmediateNodeLayout {
      Rect rect = Rect();
    } layout;

    struct UIImmediateNodeWidget {
      UIImmediateWidgetFlags flags = UIImmediateWidgetFlags::None;

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
    
    UIImmediateId hot_widget = 0;
    UIImmediateId active_widget = 0;

    uint64 current_frame_index = 0;

    Vector2 cursor_position = Vector2();

    Map<UIImmediateId, UIImmediateNode> node_map;
    UIImmediateNode root_node;
    Array<UIImmediateNode *> node_stack;
    
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
  UIImmediateNode &GetOrCreateNode(UIImmediateId id, UIImmediateWidgetFlags widget_flags) {
    UIImmediateNode *node = nullptr;

    // Try to get cached node.
    auto it = g_state.node_map.Find(id);
    if (it == g_state.node_map.end()) {
      UIImmediateNode new_node = { };
      new_node.id.id = id;
      new_node.widget.flags = widget_flags;
      g_state.node_map.Insert(id, new_node);
      node = &g_state.node_map.Get(id);
    } else {
      node = &it->second;
    }
    node->id.last_frame_touched_index = g_state.current_frame_index;

    // Put node into hierarchy by appending it as a child.
    UIImmediateNode &parent = *g_state.node_stack.GetLast();
    parent.hierarchy.child_count++;
    if (parent.hierarchy.first_child == nullptr) {
      parent.hierarchy.first_child = node;
    }
    if (parent.hierarchy.last_child != nullptr) {
      parent.hierarchy.last_child->hierarchy.next_sibling = node;
    }
    node->hierarchy.previous_sibling = parent.hierarchy.last_child; 
    parent.hierarchy.last_child = node;

    return *node;
  }

  //--------------------------------------------------------------
  UIImmediateInteraction InteractWithNode(UIImmediateNode &node) {
    if ((node.widget.flags & UIImmediateWidgetFlags::Interactable) == UIImmediateWidgetFlags::Interactable) {
      UIImmediateInteraction interaction;
      
      UIImmediateId id = node.id.id;
      
      bool8 is_inside = IsInsideRect(node.layout.rect, g_state.mouse_position);
      if (is_inside) {
        g_state.hot_widget = id;
        if (g_state.active_widget == 0 && (g_state.is_left_mouse_down || g_state.is_right_mouse_down)) {
          g_state.active_widget = id;
        }
      }

      bool8 is_hot_widget = g_state.hot_widget == id;
      bool8 is_active_widget = g_state.active_widget == id;

      interaction.hovered = is_hot_widget;
      interaction.clicked = g_state.is_left_mouse_up && is_hot_widget && is_active_widget;
      interaction.right_clicked = g_state.is_right_mouse_up && is_hot_widget && is_active_widget;
      
      return interaction;
    }

    return UIImmediateInteraction();
  }
  
  //--------------------------------------------------------------
  void IterateHierarchy(UIImmediateNode &parent, const std::function<void(UIImmediateNode &)> &callback) {
    callback(parent);

    UIImmediateNode *child = parent.hierarchy.first_child;
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

    UIImmediateLayout layout = { };
    layout.full_pivot = layout.leftover_pivot = Vector2(0.5f, 0.5f);
    layout.full_size = layout.leftover_size = Vector2(static_cast<float32>(Display::GetWidth()), static_cast<float32>(Display::GetHeight()));
    layout.full_position = layout.leftover_position = Vector3();
    s_layout_stack.Add(layout);

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
    g_state.hot_widget = 0;
    g_state.current_frame_index++;
    g_state.cursor_position = Vector2(-static_cast<float32>(Display::GetWidth()) / 2.0f, static_cast<float32>(Display::GetHeight()) / 2.0f);

    g_state.root_node = UIImmediateNode();
    g_state.node_stack.Add(&g_state.root_node);
  }

  //--------------------------------------------------------------
  void UIImmediate::End() {
    g_state.node_stack.RemoveLast();
    
    Flush();

    Layout();
    Render();
    
    if (g_state.is_left_mouse_up) {
      g_state.active_widget = 0;
    }
  }

  //--------------------------------------------------------------
  void UIImmediate::BeginWindow(const String &name, DockingPosition docking_position, float32 extent, Color color, bool8 split) {
    Vector2 anchor_min = Vector2(0.5f, 0.5f);
    Vector2 anchor_max = Vector2(0.5f, 0.5f);
    Vector2 pivot = Vector2(0.5f, 0.5f);

    Vector2 size = Vector2(extent, extent);
    
    UIImmediateLayout parent_layout = s_layout_stack.GetLast();
    Vector2 parent_pivot = parent_layout.leftover_pivot;
    Vector2 parent_size = parent_layout.leftover_size;
    Vector2 half_parent_size = parent_size / 2.0f;

    Vector3 leftover_position = parent_layout.leftover_position;
    Vector2 leftover_size = parent_layout.leftover_size;

    switch (docking_position) {
      case DockingPosition::Full: {
        anchor_min = Vector2(0.0f, 0.0f);
        anchor_max = Vector2(1.0f, 1.0f);
        pivot = Vector2(0.5f, 0.5f);
        size = Vector2(0.0f, 0.0f);
        break;
      }
      case DockingPosition::Top: {
        anchor_min = Vector2(0.0f, 1.0f);
        anchor_max = Vector2(1.0f, 1.0f);
        pivot = Vector2(0.5f, 1.0f);
        size.x = 0.0f;
        extent = split ? half_parent_size.y : extent;
        size.y = extent;
        leftover_size.y -= extent;
        leftover_position.y -= extent / 2.0f;
        break;
      }
      case DockingPosition::Bottom: {
        anchor_min = Vector2(0.0f, 0.0f);
        anchor_max = Vector2(1.0f, 0.0f);
        pivot = Vector2(0.5f, 0.0f);
        size.x = 0.0f;
        extent = split ? half_parent_size.y : extent;
        size.y = extent;
        leftover_size.y -= extent;
        leftover_position.y += extent / 2.0f;
        break;
      }
      case DockingPosition::Left: {
        anchor_min = Vector2(0.0f, 0.0f);
        anchor_max = Vector2(0.0f, 1.0f);
        pivot = Vector2(0.0f, 0.5f);
        extent = split ? half_parent_size.x : extent;
        size.x = extent;
        size.y = 0.0f;
        leftover_size.x -= extent;
        leftover_position.x += extent / 2.0f;
        break;
      }
      case DockingPosition::Right: {
        anchor_min = Vector2(1.0f, 0.0f);
        anchor_max = Vector2(1.0f, 1.0f);
        pivot = Vector2(1.0f, 0.5f);
        extent = split ? half_parent_size.x : extent;
        size.x = extent;
        size.y = 0.0f;
        leftover_size.x -= extent;
        leftover_position.x -= extent / 2.0f;
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }

    float32 anchor_x = anchor_max.x - anchor_min.x;
    float32 anchor_y = anchor_max.y - anchor_min.y;
    float32 anchor_x_size = anchor_x * parent_size.x;
    float32 anchor_y_size = anchor_y * parent_size.y;
    size.x += anchor_x * parent_size.x;
    size.y += anchor_y * parent_size.y;
    Rect rect = Rect(-pivot * size, size);

    // First we have our local position which acts as a simple offset.
    Vector3 final_local_position = Vector3(0.0f, 0.0f, 0.0f);
    // We have to take into account the parent pivot which has an effect on our local position.
    final_local_position.x += (1.0f - parent_pivot.x) * parent_size.x - half_parent_size.x;
    final_local_position.y += (1.0f - parent_pivot.y) * parent_size.y - half_parent_size.y;
    // Now we need to take into account our own pivot.
    final_local_position.x += anchor_min.x * parent_size.x - half_parent_size.x;
    final_local_position.y += anchor_min.y * parent_size.y - half_parent_size.y;
    // Add the anchoring.
    final_local_position.x += pivot.x * anchor_x_size;
    final_local_position.y += pivot.y * anchor_y_size;

    Vector3 derived_position = final_local_position;
    derived_position += parent_layout.leftover_position;
    
    Matrix4x4 transform = Matrix4x4::Translate(derived_position);

    Vector2 min = rect.GetMin();
    Vector2 max = rect.GetMax();

    Vector3 corners[4];
    corners[0] = Vector3(max.x, max.y, 0.0f);
    corners[1] = Vector3(max.x, min.y, 0.0f);
    corners[2] = Vector3(min.x, min.y, 0.0f);
    corners[3] = Vector3(min.x, max.y, 0.0f);

    s_mesh_builder.AddQuad(corners, color);
    s_mesh_builder.Transform(transform);

    UIImmediateLayout &layout = s_layout_stack.GetLast();
    layout.leftover_size = leftover_size;
    layout.leftover_position = leftover_position;

    UIImmediateLayout new_layout = { };
    new_layout.full_size = new_layout.leftover_size = size;
    new_layout.full_pivot = new_layout.leftover_pivot = pivot;
    new_layout.full_position = new_layout.leftover_position = derived_position;
    s_layout_stack.Add(new_layout);
    
    Flush();
  }
  
  //--------------------------------------------------------------
  void UIImmediate::EndWindow() {
    s_layout_stack.RemoveLast();
  }

  //--------------------------------------------------------------
  void UIImmediate::Text(const String &text) {
    UIImmediateNode &node = GetOrCreateNode(GetId(text), UIImmediateWidgetFlags::DrawText | UIImmediateWidgetFlags::DrawShadow);
    
    TextSize text_size = g_state.font->GetTextSize(StringUtils::GetCodepointsFromUtf8(text), 0, 1.0f, false);
    Vector2 size = Vector2(text_size.width, text_size.height + text_size.baseline_offset);
    Vector2 position = g_state.cursor_position;
    position.y -= size.y;
    Rect rect = Rect(position, size);

    node.layout.rect = rect;
    node.widget.text = text;
    node.widget.text_alignment = TextAlignment::TopLeft;

    AdvanceCursor(size);
  }

  //--------------------------------------------------------------
  UIImmediateInteraction UIImmediate::Button(const String &text) {
    UIImmediateId id = GetId(text);
    UIImmediateWidgetFlags flags = UIImmediateWidgetFlags::Interactable
      | UIImmediateWidgetFlags::DrawBackground
      | UIImmediateWidgetFlags::DrawText
      | UIImmediateWidgetFlags::DrawShadow;
    UIImmediateNode &node = GetOrCreateNode(id, flags);
    
    UIImmediateInteraction interaction = InteractWithNode(node);
    
    TextSize text_size = g_state.font->GetTextSize(StringUtils::GetCodepointsFromUtf8(text), 0, 1.0f, false);
    Vector2 size = Vector2(text_size.width + 10.0f, text_size.height + 8.0f);
    Vector2 position = g_state.cursor_position;
    position.y -= size.y;
    Rect rect = Rect(position, size);
    
    node.layout.rect = rect;
    node.widget.text = text;
    node.widget.text_alignment = TextAlignment::MiddleCenter;

    AdvanceCursor(size);

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
  void UIImmediate::Layout() {
    
  }

  //--------------------------------------------------------------
  void UIImmediate::Render() {
    // TODO: Color, font and shadow info should be part of a style.
    IterateHierarchy(g_state.root_node, [](UIImmediateNode &node) {
      if ((node.widget.flags & UIImmediateWidgetFlags::DrawBackgroundShadow) == UIImmediateWidgetFlags::DrawBackgroundShadow) {
        Rect rect = node.layout.rect;
        rect.position.x += 1.0f;
        rect.position.y += 1.0f;
        DrawRect(rect, Color::Black());
      }
      if ((node.widget.flags & UIImmediateWidgetFlags::DrawBackground) == UIImmediateWidgetFlags::DrawBackground) {
        DrawRect(node.layout.rect, Color::Blue());
      }
      Flush();

      if ((node.widget.flags & UIImmediateWidgetFlags::DrawShadow) == UIImmediateWidgetFlags::DrawShadow) {
        Rect rect = node.layout.rect;
        rect.position.x += 1.0f;
        rect.position.y -= 1.0f;
        DrawText(rect, node.widget.text, g_state.font, node.widget.text_alignment, Color::Black());
      }
      if ((node.widget.flags & UIImmediateWidgetFlags::DrawText) == UIImmediateWidgetFlags::DrawText) {
        DrawText(node.layout.rect, node.widget.text, g_state.font, node.widget.text_alignment, Color::White());
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
  
}
