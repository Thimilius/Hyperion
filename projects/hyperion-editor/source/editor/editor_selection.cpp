//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_selection.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    Object *EditorSelection::GetSelectedObject() {
        return ObjectManager::Get(s_selected_object_id);
    }

    //--------------------------------------------------------------
    Entity *EditorSelection::GetSelectedEntity() {
        Object *selected_object = GetSelectedObject();
        if (selected_object != nullptr && selected_object->GetType() == Type::Get<Entity>()) {
            return static_cast<Entity *>(selected_object);
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    void EditorSelection::SetSelectedObject(Object *object) {
        ObjectId object_id = 0;
        if (object != nullptr) {
            object_id = object->GetId();
        }

        if (s_selected_object_id == object_id) {
            return;
        }

        // We explicitly allow a nullptr here, meaning no selection.
        s_selected_object_id = object_id;

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