//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/widget.hpp"

#include "hyperion/ui/ui_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    void Widget::OnCreate() {
        Behaviour::OnCreate();

        UiEngine::RegisterWidget(this);
    }

    void Widget::OnDestroy() {
        UiEngine::UnregisterWidget(this);

        Behaviour::OnDestroy();
    }

}