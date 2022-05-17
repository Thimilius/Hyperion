//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/ecs/component/components/components.hpp>
#include <hyperion/ui/ui_factory.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  class EditorUI final {
  public:
    static void Initialize();
    static void Update();

    inline static UI::UIElement *GetPreviewElement() { return s_preview_container_ui_element; }
    static RectInt GetPreviewRect();
  private:
    EditorUI() = delete;
    ~EditorUI() = delete;
  private:
    static void OnEntityCreated(EntityManager *manager, EntityId id);
    static void OnEntityDestroyed(EntityManager *manager, EntityId id);
    static void OnEditorSelection(EntityId old_selection, EntityId new_selection);

    static void UpdateStatsLabel();
    static void UpdateHierarchyLabel();
    static void UpdateHierarchyLabelBranch(EntityManager *manager, EntityId branch, HierarchyComponent *branch_hierarchy, uint32 depth);
    static void UpdateSelectionLabel();

    static void UpdateSystems();
  private:
    inline static Font *s_font_text;
    inline static Font *s_font_icon;

    inline static UI::UIViewComponent s_ui_view;

    inline static UI::UIElement *s_root_element;
    inline static UI::UIElement *s_preview_container_ui_element;
    inline static UI::UIElement *s_preview_runtime_ui_element;
    inline static UI::UIElement *s_preview_editor_ui_element;

    inline static UI::UIToggle *s_toggle_vsync;
    inline static UI::UIToggle *s_toggle_grid;
    inline static UI::UIToggle *s_toggle_grounds;
    inline static UI::UIToggle *s_toggle_runtime_preview;
    inline static UI::UIToggle *s_toggle_editor_preview;

    inline static UI::UILabel *s_label_stats;
    inline static UI::UILabel *s_label_properties;
    inline static UI::UILabel *s_label_hierarchy;

    inline static UI::UIElement *s_hierarchy_container;

    inline static Map<EntityId, UI::UIButton *> s_hierarchy;
  };

}
