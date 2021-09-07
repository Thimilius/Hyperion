#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/frame/render_frame_camera.hpp"
#include "hyperion/rendering/frame/render_frame_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderFrame final {
    public:
        void Reset();

        const List<RenderFrameCamera> &GetRenderCameras() const { return m_frame_cameras; }
        const List<RenderFrameObject> &GetRenderObjects() const { return m_frame_objects; }

        RenderFrameCameraData &AddRenderCamera();
        RenderFrameObjectData &AddRenderObject();
    private:
        List<RenderFrameCamera> m_frame_cameras;
        List<RenderFrameObject> m_frame_objects;
    };

}