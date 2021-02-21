//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void UiEngine::LateUpdate() {
        for (Widget *widget : s_widgets) {
            if (widget->IsDirty()) {
                s_mesh_builder.Clear();
                widget->OnRebuildMesh(s_mesh_builder);
                widget->ResetDirty();
            }
        }
    }

    //--------------------------------------------------------------
    void UiEngine::RegisterWidget(Widget *widget) {
        HYP_ASSERT(widget);
        HYP_ASSERT(std::find(s_widgets.begin(), s_widgets.end(), widget) == s_widgets.end());

        s_widgets.push_back(widget);
    }

    //--------------------------------------------------------------
    void UiEngine::UnregisterWidget(Widget *widget) {
        HYP_ASSERT(widget);
        HYP_ASSERT(std::find(s_widgets.begin(), s_widgets.end(), widget) != s_widgets.end());

        auto begin = s_widgets.begin();
        auto end = s_widgets.end();
        if (std::find(begin, end, widget) != end) {
            s_widgets.erase(std::remove(begin, end, widget));
        }
    }

}