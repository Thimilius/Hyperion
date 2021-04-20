#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class OpenGLGraphicsBufferView : public GraphicsBufferView {
    public:
        OpenGLGraphicsBufferView(GraphicsDevice *device, const GraphicsBufferViewDescription &description, GraphicsBuffer *buffer);
    };

    class OpenGLGraphicsBuffer : public GraphicsBuffer {
    public:
        OpenGLGraphicsBuffer(GraphicsDevice *device, const GraphicsBufferDescription &description);
        ~OpenGLGraphicsBuffer();
    public:
        GraphicsBufferView *CreateView(const GraphicsBufferViewDescription &description) override;
    private:
        GLuint m_buffer_id;
    };

}