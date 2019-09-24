#include "hyppch.hpp"

#include "opengl_buffer.hpp"

namespace Hyperion::Rendering {

    COpenGLVertexBuffer::COpenGLVertexBuffer(u8 *vertices, u32 size) {
        glCreateBuffers(1, &m_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    COpenGLVertexBuffer::~COpenGLVertexBuffer() {
        glDeleteBuffers(1, &m_buffer_id);
    }

    void COpenGLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_buffer_id);
    }

    void COpenGLVertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    COpenGLIndexBuffer::COpenGLIndexBuffer(u16 *indices, u32 count) : CIndexBuffer(EIndexFormat::UInt16, count) {
        glCreateBuffers(1, &m_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u16), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    COpenGLIndexBuffer::COpenGLIndexBuffer(u32 *indices, u32 count) : CIndexBuffer(EIndexFormat::UInt32, count) {
        glCreateBuffers(1, &m_buffer_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    COpenGLIndexBuffer::~COpenGLIndexBuffer() {
        glDeleteBuffers(1, &m_buffer_id);
    }

    void COpenGLIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_id);
    }

    void COpenGLIndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}