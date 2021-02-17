//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/rendering/canvas_renderer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    CanvasRenderer *CanvasRenderer::Create() {
        return new CanvasRenderer();
    }

}