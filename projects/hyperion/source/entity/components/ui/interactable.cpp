//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/interactable.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Interactable::SetTargetColor(Color color) {
        if (m_target_widget != nullptr) {
            m_target_widget->GetWidgetRenderer()->SetColor(color);
        }
    }

}