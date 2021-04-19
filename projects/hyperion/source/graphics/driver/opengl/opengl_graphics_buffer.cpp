//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/opengl/opengl_graphics_buffer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/opengl/opengl_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    OpenGLGraphicsBuffer::OpenGLGraphicsBuffer(GraphicsDevice *device, const GraphicsBufferDescription &description) : GraphicsBuffer(device, description) {
        glCreateBuffers(1, &m_buffer_id);
        OpenGLGraphicsUtilities::SetObjectLabel(GL_BUFFER, m_buffer_id, description);
    }

    //--------------------------------------------------------------
    OpenGLGraphicsBuffer::~OpenGLGraphicsBuffer() {
        glDeleteBuffers(1, &m_buffer_id);
    }

}