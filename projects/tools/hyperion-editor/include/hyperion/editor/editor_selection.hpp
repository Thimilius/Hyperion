#pragma once

#include <hyperion/entity/object.hpp>

namespace Hyperion::Editor {

    class EditorSelection {
    public:
        inline static Object *GetSelection() { return s_selection; }
        inline static void SetSelection(Object *selection) { s_selection = selection; }
    private:
        EditorSelection() = delete;
        ~EditorSelection() = delete;
    private:
        inline static Object *s_selection;
    };

}