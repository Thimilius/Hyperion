//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/null/null_graphics_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    NullGraphicsBuffer::NullGraphicsBuffer(GraphicsDevice *device, const GraphicsBufferDescription &description) : GraphicsBuffer(device, description) {

    }

}