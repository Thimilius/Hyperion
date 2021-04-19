#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsBuffer : public GraphicsBuffer {
    public:
        NullGraphicsBuffer(GraphicsDevice *device, const GraphicsBufferDescription &description);
    };

}