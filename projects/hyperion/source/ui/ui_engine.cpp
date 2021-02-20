//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void UiEngine::LateUpdate() {
        for (Widget *widget : m_widgets) {
            if (widget->IsDirty()) {
                widget->OnRebuildMesh();
                widget->ResetDirty();
            }
        }
    }

    //--------------------------------------------------------------
    void UiEngine::RegisterWidget(Widget *widget) {
        HYP_ASSERT(widget);
        HYP_ASSERT(std::find(m_widgets.begin(), m_widgets.end(), widget) == m_widgets.end());

        m_widgets.push_back(widget);
    }

    //--------------------------------------------------------------
    void UiEngine::UnregisterWidget(Widget *widget) {
        HYP_ASSERT(widget);
        HYP_ASSERT(std::find(m_widgets.begin(), m_widgets.end(), widget) != m_widgets.end());

        auto begin = m_widgets.begin();
        auto end = m_widgets.end();
        if (std::find(begin, end, widget) != end) {
            m_widgets.erase(std::remove(begin, end, widget));
        }
    }

}