#include "hyperion/editor/editor_selection.hpp"

namespace Hyperion::Editor {

    void EditorSelection::SetSelection(Object *selection) {
        if (s_selection != selection) {
            s_selection = selection;

            for (EditorSelectionListener *listener : s_selection_listeners) {
                listener->OnSelection(selection);
            }
        }
    }

    void EditorSelection::RegisterSelectionListener(EditorSelectionListener *selection_listener) {
        s_selection_listeners.push_back(selection_listener);
    }

    void EditorSelection::UnregisterSelectionListener(EditorSelectionListener *selection_listener) {
        auto begin = s_selection_listeners.begin();
        auto end = s_selection_listeners.end();
        if (std::find(begin, end, selection_listener) != end) {
            s_selection_listeners.erase(std::remove(begin, end, selection_listener));
        }
    }

}