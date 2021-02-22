//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/widget.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"
#include "hyperion/ui/ui_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Widget::SetColor(Color color) {
        if (m_color == color) {
            return;
        }

        m_color = color;
        SetDirty();
    }

    //--------------------------------------------------------------
    void Widget::OnCreate() {
        Behaviour::OnCreate();

        UiEngine::RegisterWidget(this);

        m_rect_transform = GetEntity()->GetComponent<RectTransform>();
        HYP_ASSERT(m_rect_transform);
        m_widget_renderer = GetEntity()->GetComponent<WidgetRenderer>();
        HYP_ASSERT(m_widget_renderer);
    }

    //--------------------------------------------------------------
    void Widget::OnDestroy() {
        UiEngine::UnregisterWidget(this);

        Behaviour::OnDestroy();
    }

}