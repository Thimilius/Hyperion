#pragma once

#include <hyperion/core/object.hpp>

namespace Hyperion::Editor {

    class IEditorSelectionListener {
    public:
        virtual ~IEditorSelectionListener() = default;

        virtual void OnSelection(Object *selection) = 0;
    };

    class EditorSelection {
        using SelectionCallbackFunction = std::function<void(Object *)>;
    public:
        inline static Object *GetSelection() { return s_selection; }
        static void SetSelection(Object *selection);

        static void RegisterSelectionListener(IEditorSelectionListener *selection_listener);
        static void UnregisterSelectionListener(IEditorSelectionListener *selection_listener);
    private:
        EditorSelection() = delete;
        ~EditorSelection() = delete;
    private:
        inline static Object *s_selection;

        inline static Vector<IEditorSelectionListener *> s_selection_listeners;
    };

}