#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_buffer.hpp"

namespace Hyperion::Rendering {

    COpenGLVertexBuffer::COpenGLVertexBuffer(const u8 *vertices, u32 size) : COpenGLVertexBuffer(vertices, size, EBufferUsage::StaticDraw) { }

    COpenGLVertexBuffer::COpenGLVertexBuffer(const u8 *vertices, u32 size, EBufferUsage usage) : CVertexBuffer(size) {
        glCreateBuffers(1, &m_vertex_buffer_id);
        glNamedBufferData(m_vertex_buffer_id, size, vertices, GetGLBufferUsage(usage));
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

    void COpenGLVertexBuffer::SetData(u32 offset, const u8 *verticies, u32 size) {
        HYP_ASSERT_MESSAGE((offset + size) < m_size, "Trying to set more data that can fit into the vertex buffer!");
        glNamedBufferSubData(m_vertex_buffer_id, offset, size, verticies);
    }

    u32 COpenGLVertexBuffer::GetGLBufferUsage(EBufferUsage usage) {
        switch (usage) {
            case EBufferUsage::StaticDraw: return GL_STATIC_DRAW;
            case EBufferUsage::StreamDraw: return GL_STREAM_DRAW;
            case EBufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
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