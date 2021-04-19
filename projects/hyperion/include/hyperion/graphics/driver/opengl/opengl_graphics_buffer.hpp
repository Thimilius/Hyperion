#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class OpenGLGraphicsBuffer : public GraphicsBuffer {
    public:
        OpenGLGraphicsBuffer(GraphicsDevice *device, const GraphicsBufferDescription &description);
    };

}