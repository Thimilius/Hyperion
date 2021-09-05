#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/view/render_view_camera.hpp"
#include "hyperion/rendering/view/render_view_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderView final {
    public:
        void AddRenderCamera(RenderViewCamera *render_view_camera);
        void AddRenderObject(RenderViewObject *render_view_object);
    };

}