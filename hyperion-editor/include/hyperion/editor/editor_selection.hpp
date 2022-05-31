//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/ecs/entity/entity_types.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  using EditorSelectionCallback = Delegate<void(EntityId, EntityId)>;

  class EditorSelection final {
  public:
    static bool8 HasSelection() { return s_selection != EntityId::EMPTY; }
    static EntityId GetSelection() { return s_selection; }

    static void Select(EntityId selection) {
      if (s_selection != selection) {
        EntityId old_selection = s_selection;
        s_selection = selection;

        for (auto callback : s_callbacks) {
          callback(old_selection, selection);
        }
      }
    }
    static void Deselect() { Select(EntityId::EMPTY); }

    static void RegisterSelectionCallback(const EditorSelectionCallback &callback) { s_callbacks.Add(callback); }
    static void UnregisterSelectionCallback(const EditorSelectionCallback &callback) { s_callbacks.Remove(callback); }
  private:
    EditorSelection() = delete;
    ~EditorSelection() = delete;
  private:
    inline static EntityId s_selection = EntityId::EMPTY;
    inline static Array<EditorSelectionCallback> s_callbacks;
  };

}
