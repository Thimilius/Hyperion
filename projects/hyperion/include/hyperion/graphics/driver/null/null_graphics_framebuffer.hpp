#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_framebuffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsFramebuffer : public GraphicsFramebuffer {
    public:
        NullGraphicsFramebuffer(GraphicsDevice *device, const GraphicsFramebufferDescription &description);
    };

}