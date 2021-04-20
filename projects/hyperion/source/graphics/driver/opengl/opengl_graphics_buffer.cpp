//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/opengl/opengl_graphics_buffer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/opengl/opengl_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    OpenGLGraphicsBufferView::OpenGLGraphicsBufferView(GraphicsDevice *device, const GraphicsBufferViewDescription &description, GraphicsBuffer *buffer) : GraphicsBufferView(device, description, buffer) {

    }

    //--------------------------------------------------------------
    OpenGLGraphicsBuffer::OpenGLGraphicsBuffer(GraphicsDevice *device, const GraphicsBufferDescription &description) : GraphicsBuffer(device, description) {
        glCreateBuffers(1, &m_buffer_id);
        OpenGLGraphicsUtilities::SetObjectLabel(GL_BUFFER, m_buffer_id, description);
    }

    //--------------------------------------------------------------
    OpenGLGraphicsBuffer::~OpenGLGraphicsBuffer() {
        glDeleteBuffers(1, &m_buffer_id);
    }

    //--------------------------------------------------------------
    GraphicsBufferView *OpenGLGraphicsBuffer::CreateView(const GraphicsBufferViewDescription &description) {
        return new OpenGLGraphicsBufferView(GetDevice(), description, this);
    }

}