//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/utilities/mesh_builder.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/ui/immediate/ui_immediate_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  class UIImmediate final {
  public:
    static void Begin();
    static void End();

    static UIImmediateTheme *GetDefaultTheme() { return &s_default_theme; }
    static UIImmediateTheme *CreateTheme(const String &name, UIImmediateTheme *template_theme = GetDefaultTheme());
    static UIImmediateTheme *GetTheme(const String &name);
    static void DestroyTheme(const String &name);

    static void SetOverlayColor(Color color) { s_overlay_color = color; }
    
    static UIImmediateId GetId(const String &text);
    static void PushId(const String &text);
    static void PopId();

    static bool8 HasFocusedElement();
    static UIImmediateElement *GetElement(UIImmediateId id);
    
    static UIImmediateInteraction BeginPanel(
      const String &text,
      Size size[2],
      ChildLayout child_layout = ChildLayout::Horizontal,
      bool8 interactable = false,
      UIImmediateTheme *theme = GetDefaultTheme()
    );
    static void EndPanel();

    static void BeginEmpty(const String &id_text);
    static void EndEmpty();
    
    static void FillSpace();
    static void Space(SizeKind kind, float32 value);

    static void BeginCenter(const String &id_text);
    static void EndCenter();

    static void Separator(UIImmediateTheme *theme = GetDefaultTheme());

    static UIImmediateInteraction Text(
      const String &text,
      TextAlignment text_alignment = TextAlignment::TopLeft,
      FitType fit_type = FitType::None,
      bool8 interactable = false,
      UIImmediateTheme *theme = GetDefaultTheme()
    );
    static UIImmediateInteraction Button(const String &text, FitType fit_type = FitType::None, UIImmediateTheme *theme = GetDefaultTheme());
    static UIImmediateInteraction TextToggle(
      bool8 &value,
      const String &text,
      FitType fit_type = FitType::None,
      UIImmediateTheme *theme = GetDefaultTheme()
    );
    static UIImmediateInteraction Input(
      const String &id_text,
      String &text,
      TextAlignment text_alignment = TextAlignment::TopLeft,
      FitType fit_type = FitType::None,
      UIImmediateTheme *theme = GetDefaultTheme()
    );
    static void Image(const String &id_text, Texture *texture, Size size[2], bool8 enable_blending = true);

    static Vector2 UISpacePointToScreenPoint(Vector2 ui_space_point);
    static Vector2 ScreenPointToUISpacePoint(Vector2 screen_point);
    static bool8 IsInsideRect(Rect rect, Vector2 screen_point);
  private:
    UIImmediate() = delete;
    ~UIImmediate() = delete;
  private:
    static void Layout();
    static void Render();

    static Color GetBackgroundColor(const UIImmediateElement &element);
    static Color GetTextColor(const UIImmediateElement &element);

    static void DrawRect(Rect rect, Color color);
    static void DrawText(Rect rect, const String &text, Font *font, UI::TextAlignment alignment, Color color, Vector2 offset = Vector2());
    static void Flush(RectInt scissor, Material *material = nullptr, Texture *texture = nullptr, bool8 affected_by_overlay = true);
    
    static bool8 IsInsideParent(const UIImmediateElement &element);

    static void FitToLayout(UIImmediateElement &element, FitType fit_type);
    static FitLayoutAxes GetLayoutAxesForParentChildLayout(const UIImmediateElement &element);
    
    static UIImmediateId HashIdText(const String &id_text);
    static String GetIdTextFromStack(const String &text);
    
    static UIImmediateElement &GetOrCreateElement(const String &id_text, UIImmediateWidgetFlags widget_flags);
    static UIImmediateElement &CreateTemporaryElement(UIImmediateWidgetFlags widget_flags);
    static void PlaceElementInHierarchy(UIImmediateElement &element);
    static UIImmediateInteraction InteractWithElement(UIImmediateElement &element);

    static void IterateHierarchy(UIImmediateElement &parent, const std::function<bool8(UIImmediateElement &)> &callback);
  private:
    inline static UIImmediateState s_state;
    
    inline static MeshBuilder s_mesh_builder;
    inline static Array<UIImmediateMeshDraw> s_mesh_draws;
    inline static std::unordered_multimap<uint32, UIImmediateMesh> s_mesh_cache;
    inline static Array<UIImmediateMesh> s_meshes_in_use;
    
    inline static UIImmediateTheme s_default_theme;
    inline static Map<uint64, UIImmediateTheme> s_themes;
    
    inline static Color s_overlay_color = Color::White();
  };

}
