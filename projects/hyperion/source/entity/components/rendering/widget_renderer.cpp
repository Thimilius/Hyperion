//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/rendering/widget_renderer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    WidgetRenderer *WidgetRenderer::Create() {
        return new WidgetRenderer();
    }

}