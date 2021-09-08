//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"
#include "hyperion/rendering/frame/render_frame.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Graphics;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Initialize(GraphicsContext *graphics_context) {
        graphics_context->CreateDeviceAndSwapChain(&m_device, &m_device_context, &m_swap_chain);
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Render(RenderFrame *render_frame) {
        for (const RenderFrameCamera &render_frame_camera : render_frame->GetRenderCameras()) {
            RenderCamera(render_frame_camera, render_frame);
        }
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Shutdown() {

    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::RenderCamera(const RenderFrameCamera &render_frame_camera, RenderFrame *render_frame) {
        const RenderFrameCameraData &render_frame_camera_data = render_frame_camera.GetData();
        const CameraViewport &viewport = render_frame_camera_data.viewport;

        // TEMP: This is just so we see something on the screen.
        glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        Color background_color = render_frame_camera_data.background_color;
        glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

}