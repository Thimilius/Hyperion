//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/null/null_graphics_framebuffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    NullGraphicsFramebuffer::NullGraphicsFramebuffer(GraphicsDevice *device, const GraphicsFramebufferDescription &description) : GraphicsFramebuffer(device, description) {

    }

}