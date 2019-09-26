#include "hyppch.hpp"

#include "opengl_buffer.hpp"

namespace Hyperion::Rendering {

    COpenGLVertexBuffer::COpenGLVertexBuffer(const u8 *vertices, u32 size) {
        glCreateBuffers(1, &m_vertex_buffer_id);
        glNamedBufferData(m_vertex_buffer_id, size, vertices, GL_STATIC_DRAW);
    }

    COpenGLVertexBuffer::~COpenGLVertexBuffer() {
        glDeleteBuffers(1, &m_vertex_buffer_id);
    }

    void COpenGLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
    }

    void COpenGLVertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    COpenGLIndexBuffer::COpenGLIndexBuffer(const u16 *indices, u32 count) : CIndexBuffer(EIndexFormat::UInt16, count) {
        glCreateBuffers(1, &m_index_buffer_id);
        glNamedBufferData(m_index_buffer_id, count * sizeof(u16), indices, GL_STATIC_DRAW);
    }

    COpenGLIndexBuffer::COpenGLIndexBuffer(const u32 *indices, u32 count) : CIndexBuffer(EIndexFormat::UInt32, count) {
        glCreateBuffers(1, &m_index_buffer_id);
        glNamedBufferData(m_index_buffer_id, count * sizeof(u32), indices, GL_STATIC_DRAW);
    }

    COpenGLIndexBuffer::~COpenGLIndexBuffer() {
        glDeleteBuffers(1, &m_index_buffer_id);
    }

    void COpenGLIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);
    }

    void COpenGLIndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}