//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/menu.hpp>
#include <hyperion/core/math/rect.hpp>
#include <hyperion/ecs/world/world.hpp>
#include <hyperion/render/render_gizmos.hpp>
#include <hyperion/render/frame/render_frame.hpp>
#include <hyperion/ui/immediate/ui_immediate_types.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  enum class EditorViewMode {
    Editor,
    Game
  };
  
  class EditorUI final {
  public:
    static void Initialize();
    static void Update();

    static bool8 HasFocusedElement();
    static Rect GetPreviewRect();
    static bool8 IsMouseInsidePreviewRect();

    static void SetTransformationTool(Rendering::RenderGizmoType transformation_tool) { s_transformation_tool = transformation_tool; }
    
    static void HandleMouseSelection(Rendering::RenderFrame *render_frame, RenderTexture *render_texture);
  private:
    EditorUI() = delete;
    ~EditorUI() = delete;
  private:
    static void HeaderPanel();
    static void EntityHierarchyPanel();
    static void EntityHierarchyPanelRecursive(EntityManager *manager, EntityId entity, HierarchyComponent *branch_hierarchy, uint32 depth);
    static void EntityInspectionPanel();
    static void PreviewPanel();

    static void ComponentPanel(const ComponentInfo &component_info, Type component_type, void *component);
    static void PropertyPanel(Instance instance, Property property);

    static void UpdateGizmo();
    
    static Vector2 TransformScreenToPreviewPosition(Vector2 screen_position);

    static void OpenContextMenu(const Menu &menu);
    static void ScheduleAction(const std::function<void()> &delayed_action);
  private:
    inline static UI::UIImmediateId s_preview_element;

    inline static UI::UIImmediateTheme *s_icon_theme;
    inline static UI::UIImmediateTheme *s_selection_theme;
    inline static UI::UIImmediateTheme *s_panel_theme;
    inline static UI::UIImmediateTheme *s_disabled_theme;
    inline static UI::UIImmediateTheme *s_disabled_icon_theme;

    inline static EditorViewMode s_view_mode = EditorViewMode::Editor;
    
    inline static bool8 s_is_in_gizmo;
    inline static Rendering::RenderGizmoType s_transformation_tool = Rendering::RenderGizmoType::Translate;

    inline static Menu s_entity_creation_menu;

    inline static Array<std::function<void()>> s_delayed_actions;
  };

}
