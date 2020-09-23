#pragma once

#include <hyperion/entity/object.hpp>

namespace Hyperion::Editor {

    class EditorSelectionListener {
    public:
        virtual ~EditorSelectionListener() = default;

        virtual void OnSelection(Object *selection) = 0;
    };

    class EditorSelection {
        using SelectionCallbackFunction = std::function<void(Object *)>;
    public:
        inline static Object *GetSelection() { return s_selection; }
        static void SetSelection(Object *selection);

        static void RegisterSelectionListener(EditorSelectionListener *selection_listener);
        static void UnregisterSelectionListener(EditorSelectionListener *selection_listener);
    private:
        EditorSelection() = delete;
        ~EditorSelection() = delete;
    private:
        inline static Object *s_selection;

        inline static Vector<EditorSelectionListener *> s_selection_listeners;
    };

}