//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/menu.hpp>
#include <hyperion/core/math/rect.hpp>
#include <hyperion/ecs/world/world.hpp>
#include <hyperion/render/render_gizmos.hpp>
#include <hyperion/render/frame/render_frame.hpp>
#include <hyperion/ui/immediate/ui_immediate_types.hpp>
#include <hyperion/ui/immediate/ui_immediate_gizmos.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  enum class EditorViewMode {
    Editor,
    Game
  };
  
  class EditorUI final {
  public:
    static void Initialize();
    static void Draw();

    static EditorViewMode GetViewMode() { return s_view_mode; }
    
    static bool8 HasFocusedElement();
    static Rect GetPreviewRect();
    static bool8 IsMouseInsidePreviewRect();
    
    static void SetTransformationTool(Rendering::RenderGizmoType transformation_tool) { s_transformation_tool = transformation_tool; }
    static void SetTransformationMode(UI::GizmoMode transformation_mode) { s_transformation_mode = transformation_mode; }
    
    static void HandleMouseSelection(Rendering::RenderFrame *render_frame, RenderTexture *render_texture);
  private:
    EditorUI() = delete;
    ~EditorUI() = delete;
  private:
    static void HeaderPanel();
    static void EntityHierarchyPanel();
    static void EntityHierarchyPanelRecursive(EntityManager *manager, EntityId entity, HierarchyComponent *branch_hierarchy, uint32 depth);
    static void EntityInspectorPanel();
    static void PreviewPanel();
    static void ConsolePanel();

    static void ComponentPanel(const ComponentInfo &component_info, Type component_type, void *component);
    static void PropertyPanel(Instance instance, Property property);
    static bool8 PropertyVector3(Property property, Instance instance, Vector3 value);
    static bool8 PropertyQuaternion(Property property, Instance instance, Quaternion value);
    static bool8 PropertyEnum(Property property, Instance instance, Type property_type, Variant property_value);

    static void UpdateGizmo();

    static void OnSelectionChange(EntityId old_entity, EntityId new_entity);
    
    static Vector2 TransformScreenToPreviewPosition(Vector2 screen_position);

    static void CreateEntityMenu(Array<MenuItem> &items);
    static void OpenContextMenu(const Menu &menu);
    static void ScheduleAction(const std::function<void()> &delayed_action);
  private:
    inline static UI::UIImmediateElement *s_preview_element;
    inline static UI::UIImmediateElement *s_inspector_element;

    inline static UI::UIImmediateTheme *s_icon_theme;
    inline static UI::UIImmediateTheme *s_selection_theme;
    inline static UI::UIImmediateTheme *s_panel_theme;
    inline static UI::UIImmediateTheme *s_no_toggle_highlight_icon_theme;
    inline static UI::UIImmediateTheme *s_entity_disabled_theme;
    inline static UI::UIImmediateTheme *s_entity_disabled_icon_theme;

    inline static EditorViewMode s_view_mode = EditorViewMode::Editor;

    inline static Map<Type, bool8> s_component_panel_toggles;
    
    inline static bool8 s_is_in_gizmo;
    inline static Rendering::RenderGizmoType s_transformation_tool = Rendering::RenderGizmoType::Translate;
    inline static UI::GizmoMode s_transformation_mode = UI::GizmoMode::Local;

    inline static Array<std::function<void()>> s_delayed_actions;
  };

}
