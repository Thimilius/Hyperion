//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/widget.hpp"

//---------------------- Project Includes ----------------------
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
    }

    //--------------------------------------------------------------
    void Widget::OnDestroy() {
        UiEngine::UnregisterWidget(this);

        Behaviour::OnDestroy();
    }

}