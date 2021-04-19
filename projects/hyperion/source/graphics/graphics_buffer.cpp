//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/graphics_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    GraphicsBuffer::GraphicsBuffer(GraphicsDevice *device, const GraphicsBufferDescription &description) : GraphicsDeviceObject(device) {
        m_description = description;
    }

}