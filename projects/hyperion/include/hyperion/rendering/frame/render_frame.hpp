#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/frame/render_frame_camera.hpp"
#include "hyperion/rendering/frame/render_frame_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderFrame final {
    public:
        void Reset();

        const Array<RenderFrameCamera> &GetFrameCameras() const { return m_frame_cameras; }
        const Array<RenderFrameObject> &GetFrameObjects() const { return m_frame_objects; }

        RenderFrameCameraData &AddFrameCamera();
        RenderFrameObjectData &AddFrameObject();
    private:
        Array<RenderFrameCamera> m_frame_cameras;
        Array<RenderFrameObject> m_frame_objects;
    };

}