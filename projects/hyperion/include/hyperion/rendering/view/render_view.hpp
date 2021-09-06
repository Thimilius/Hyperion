#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/view/render_view_camera.hpp"
#include "hyperion/rendering/view/render_view_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderView final {
    public:
        void Reset();

        RenderViewCameraData &AddRenderCamera();
        RenderViewObjectData &AddRenderObject();
    private:
        List<RenderViewCamera> m_view_cameras;
        List<RenderViewObject> m_view_objects;
    };

}