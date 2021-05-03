//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/graphics_framebuffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    GraphicsFramebuffer::GraphicsFramebuffer(GraphicsDevice *device, const GraphicsFramebufferDescription &description) : GraphicsDeviceObject(device) {
        m_description = description;
    }

}