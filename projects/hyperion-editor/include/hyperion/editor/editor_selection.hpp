#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/object/object.hpp>
#include <hyperion/entity/entity.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class IEditorSelectionChangedListener {
    public:
        ~IEditorSelectionChangedListener() = default;
    public:
        virtual void OnSelectionChanaged(Object *selection) = 0;
    };

    class EditorSelection final {
    public:
        static Object *GetSelectedObject();
        static Entity *GetSelectedEntity();

        static void SetSelectedObject(Object *object);

        static void RegisterSelectionChangedListener(IEditorSelectionChangedListener *selection_changed_listener);
        static void UnregisterSelectionChangedListener(IEditorSelectionChangedListener *selection_changed_listener);
    private:
        inline static ObjectId s_selected_object_id;
        inline static Vector<IEditorSelectionChangedListener *> s_selection_changed_listeners;
    };

}