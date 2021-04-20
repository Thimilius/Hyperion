#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsBufferView : public GraphicsBufferView {
    public:
        NullGraphicsBufferView(GraphicsDevice *device, const GraphicsBufferViewDescription &description, GraphicsBuffer *buffer);
    };

    class NullGraphicsBuffer : public GraphicsBuffer {
    public:
        NullGraphicsBuffer(GraphicsDevice *device, const GraphicsBufferDescription &description);
    public:
        GraphicsBufferView *CreateView(const GraphicsBufferViewDescription &description) override;
    };

}