//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/null/null_graphics_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    NullGraphicsBufferView::NullGraphicsBufferView(GraphicsDevice *device, const GraphicsBufferViewDescription &description, GraphicsBuffer *buffer) : GraphicsBufferView(device, description, buffer) {

    }

    //--------------------------------------------------------------
    NullGraphicsBuffer::NullGraphicsBuffer(GraphicsDevice *device, const GraphicsBufferDescription &description) : GraphicsBuffer(device, description) {

    }

    //--------------------------------------------------------------
    GraphicsBufferView *NullGraphicsBuffer::CreateView(const GraphicsBufferViewDescription &description) {
        return new NullGraphicsBufferView(GetDevice(), description, this);
    }

}