//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/utilities/mesh_builder.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/ui/immediate/ui_immediate_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::UI {
  class UIElement;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  class UIImmediate final {
  public:
    static void Begin();
    static void End();

    static UIImmediateTheme *GetDefaultTheme() { return &s_default_theme; }
    static UIImmediateTheme *CreateTheme(const String &name);
    static UIImmediateTheme *GetTheme(const String &name);
    static void DestroyTheme(const String &name);

    static void PushId(const String &text);
    static void PopId();
    
    static void BeginPanel(const String &text, Size size[2], ChildLayout child_layout = ChildLayout::Horizontal, UIImmediateTheme *theme = GetDefaultTheme());
    static void EndPanel();

    static void BeginEmpty();
    static void EndEmpty();
    
    static void FillSpace();
    static void Space(SizeKind kind, float32 value);

    static void BeginCenter();
    static void EndCenter();

    static void Separator(UIImmediateTheme *theme = GetDefaultTheme());

    static void Text(
      const String &text,
      TextAlignment text_alignment = TextAlignment::TopLeft,
      bool8 fit_to_parent = false,
      UIImmediateTheme *theme = GetDefaultTheme()
    );
    static UIImmediateInteraction Button(const String &text, bool8 fit_to_parent = false, UIImmediateTheme *theme = GetDefaultTheme());
  private:
    UIImmediate() = delete;
    ~UIImmediate() = delete;
  private:
    static void Layout();
    static void Render();

    static void DrawRect(Rect rect, Color color);
    static void DrawText(Rect rect, const String &text, Font *font, UI::TextAlignment alignment, Color color);
    static void Flush(Material *material = nullptr, Texture *texture = nullptr);

    static LayoutAxes GetAxesForParentLayout(UIImmediateElement &element);
    
    static UIImmediateId GetId(const String &text);
    
    static Vector2 ScreenPointToUISpacePoint(Vector2 screen_point);
    static bool8 IsInsideRect(Rect rect, Vector2 screen_point);

    static UIImmediateElement &GetOrCreateElement(UIImmediateId id, UIImmediateWidgetFlags widget_flags);
    static UIImmediateElement &CreateTemporaryElement(UIImmediateWidgetFlags widget_flags);
    static void PlaceElementInHierarchy(UIImmediateElement &element);
    static UIImmediateInteraction InteractWithElement(const UIImmediateElement &element);

    static void IterateHierarchy(UIImmediateElement &parent, const std::function<void(UIImmediateElement &)> &callback);
  private:
    inline static UIImmediateState s_state;
    
    inline static MeshBuilder s_mesh_builder;
    inline static Array<UIImmediateMesh> s_meshes;

    inline static UIImmediateTheme s_default_theme;
    inline static Map<uint64, UIImmediateTheme> s_themes;
  };

}
