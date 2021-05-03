#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_framebuffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class OpenGLGraphicsFramebuffer : public GraphicsFramebuffer {
    public:
        OpenGLGraphicsFramebuffer(GraphicsDevice *device, const GraphicsFramebufferDescription &description);
    };

}