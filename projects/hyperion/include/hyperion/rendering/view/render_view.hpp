#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/view/render_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class RenderView final {
    public:
        void AddRenderObject(RenderObject *render_object);
    };

}