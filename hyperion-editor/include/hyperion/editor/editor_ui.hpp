//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
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

    static RectInt GetPreviewRect();

    static void HandleMouseSelection(Rendering::RenderFrame *render_frame, RenderTexture *render_texture);
  private:
    EditorUI() = delete;
    ~EditorUI() = delete;
  private:
    static void DrawEntityHierarchy(EntityManager *manager, EntityId entity, HierarchyComponent *branch_hierarchy, uint32 depth);
  private:
    inline static UI::UIImmediateId s_preview_element;

    inline static UI::UIImmediateTheme *s_icon_theme;
    inline static UI::UIImmediateTheme *s_selection_theme;
  };

}
