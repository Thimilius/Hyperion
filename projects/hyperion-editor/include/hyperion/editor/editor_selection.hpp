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
        inline static Object *GetSelectedObject() { return s_selected_object; }
        static Entity *GetSelectedEntity() { return s_selected_object->GetType() == Type::get<Entity>() ? static_cast<Entity *>(s_selected_object) : nullptr; }

        static void SetSelectedObject(Object *object);

        static void RegisterSelectionChangedListener(IEditorSelectionChangedListener *selection_changed_listener);
        static void UnregisterSelectionChangedListener(IEditorSelectionChangedListener *selection_changed_listener);
    private:
        inline static Object *s_selected_object;
        inline static Vector<IEditorSelectionChangedListener *> s_selection_changed_listeners;
    };

}