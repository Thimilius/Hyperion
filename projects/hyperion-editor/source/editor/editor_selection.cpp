//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_selection.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorSelection::SetSelectedObject(Object *object) {
        if (s_selected_object == object) {
            return;
        }

        // We explicitly allow a nullptr here, meaning no selection.
        s_selected_object = object;

        for (IEditorSelectionChangedListener *selection_changed_listener : s_selection_changed_listeners) {
            selection_changed_listener->OnSelectionChanaged(object);
        }
    }

    //--------------------------------------------------------------
    void EditorSelection::RegisterSelectionChangedListener(IEditorSelectionChangedListener *selection_changed_listener) {
        HYP_ASSERT(selection_changed_listener);

        s_selection_changed_listeners.push_back(selection_changed_listener);
    }

    //--------------------------------------------------------------
    void EditorSelection::UnregisterSelectionChangedListener(IEditorSelectionChangedListener *selection_changed_listener) {
        HYP_ASSERT(selection_changed_listener);

        auto begin = s_selection_changed_listeners.begin();
        auto end = s_selection_changed_listeners.end();
        if (std::find(begin, end, selection_changed_listener) != end) {
            s_selection_changed_listeners.erase(std::remove(begin, end, selection_changed_listener));
        }
    }

}