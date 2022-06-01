//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/menu.hpp>
#include <hyperion/core/math/rect.hpp>
#include <hyperion/ecs/world/world.hpp>
#include <hyperion/render/frame/render_frame.hpp>
#include <hyperion/ui/immediate/ui_immediate_types.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  class EditorUI final {
  public:
    static void Initialize();
    static void Update();

    static bool8 HasFocusedElement();
    static Rect GetPreviewRect();
    static bool8 IsMouseInsidePreviewRect();

    static void HandleMouseSelection(Rendering::RenderFrame *render_frame, RenderTexture *render_texture);
  private:
    EditorUI() = delete;
    ~EditorUI() = delete;
  private:
    static void DrawEntityHierarchy(EntityManager *manager, EntityId entity, HierarchyComponent *branch_hierarchy, uint32 depth);

    static void CreateEntity(EntityPrimitive primitive);
    
    static Vector2 TransformScreenToPreviewPosition(Vector2 screen_position);
  private:
    inline static UI::UIImmediateId s_preview_element;

    inline static UI::UIImmediateTheme *s_icon_theme;
    inline static UI::UIImmediateTheme *s_selection_theme;
    inline static UI::UIImmediateTheme *s_panel_theme;

    inline static bool8 s_is_in_gizmo;

    inline static Menu s_entity_creation_menu;
  };

}
