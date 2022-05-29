//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/utilities/mesh_builder.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/ui/ui_event.hpp"
#include "hyperion/ui/ui_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::UI {
  class UIElement;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  struct UIImmediateMesh {
    Mesh *mesh;
    Material *material;
    Texture *texture;

    Color color;
    bool8 enable_blending;
    
    uint32 render_texture_attachment_index;
  };

  struct UIImmediateInteraction {
    bool8 hovered = false;
    bool8 clicked = false;
    bool8 right_clicked = false;
  };

  enum class UIImmediateSizeKind {
    None,
    Pixels,
    TextContent,
    PercentOfParent,
    ChildrenSum,
    AutoFill
  };
  
  struct UIImmediateSize {
    UIImmediateSizeKind kind;
    float32 value;
  };

  enum class UIImmediateChildLayout {
    Horizontal,
    Vertical
  };

  struct UIImmediateTheme {
    Color background_color = Color::Grey();
    Color background_color_hover = Color::Grey();
    Color background_color_pressed = Color::Grey();
    
    Color text_color = Color::White();
    Color text_color_hover = Color::White();
    Color text_color_pressed = Color::White();

    Color shadow_color = Color::Black();
    Vector2 shadow_offset = Vector2(1.0f, -1.0f);
  };
  
  class UIImmediate final {
  public:
    static void Begin();
    static void End();

    static void SetPanelTheme(const UIImmediateTheme &theme) { s_panel_theme = theme; }
    static void SetTextTheme(const UIImmediateTheme &theme) { s_text_theme = theme; }
    static void SetButtonTheme(const UIImmediateTheme &theme) { s_button_theme = theme; }
    
    static void BeginPanel(const String &text, UIImmediateSize size[2], UIImmediateChildLayout child_layout);
    static void EndPanel();

    static void BeginEmpty();
    static void EndEmpty();
    
    static void FillSpace();
    static void Space(UIImmediateSizeKind kind, float32 value);

    static void BeginCenter();
    static void EndCenter();

    static void Text(const String &text);
    static UIImmediateInteraction Button(const String &text);
    
    static void DrawRect(Rect rect, Color color);
    static void DrawText(Rect rect, const String &text, Font *font, UI::TextAlignment alignment, Color color);
  private:
    UIImmediate() = delete;
    ~UIImmediate() = delete;
  private:
    static void Layout();
    static void Render();

    static void Flush(Material *material = nullptr, Texture *texture = nullptr);
  private:
    inline static MeshBuilder s_mesh_builder;
    inline static Array<UIImmediateMesh> s_meshes;

    inline static UIImmediateTheme s_panel_theme;
    inline static UIImmediateTheme s_text_theme;
    inline static UIImmediateTheme s_button_theme;
  };

}
