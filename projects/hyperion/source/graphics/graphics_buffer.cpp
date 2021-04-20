//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/graphics_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    GraphicsBufferView::GraphicsBufferView(GraphicsDevice *device, const GraphicsBufferViewDescription &description, GraphicsBuffer *buffer) : GraphicsDeviceObject(device) {
        m_description = description;
        m_buffer = buffer;
    }

    //--------------------------------------------------------------
    GraphicsBuffer::GraphicsBuffer(GraphicsDevice *device, const GraphicsBufferDescription &description) : GraphicsDeviceObject(device) {
        m_description = description;
    }

}