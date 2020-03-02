#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_buffer.hpp"

#include <glad/glad.h>

namespace Hyperion::Rendering {

    OpenGLVertexBuffer::OpenGLVertexBuffer(const u8 *vertices, u32 size) : OpenGLVertexBuffer(vertices, size, BufferUsage::StaticDraw) { }

    OpenGLVertexBuffer::OpenGLVertexBuffer(const u8 *vertices, u32 size, BufferUsage usage) : VertexBuffer(size) {
        glCreateBuffers(1, &m_vertex_buffer_id);
        glNamedBufferData(m_vertex_buffer_id, size, vertices, GetGLBufferUsage(usage));
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        glDeleteBuffers(1, &m_vertex_buffer_id);
    }

    void OpenGLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_id);
    }

    void OpenGLVertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::SetData(u32 offset, u32 size, const u8 *verticies) {
        HYP_ASSERT_MESSAGE((offset + size) <= m_size, "Trying to set more data that can fit into the vertex buffer!");
        glNamedBufferSubData(m_vertex_buffer_id, offset, size, verticies);
    }

    u32 OpenGLVertexBuffer::GetGLBufferUsage(BufferUsage usage) {
        switch (usage) {
            case BufferUsage::StaticDraw: return GL_STATIC_DRAW;
            case BufferUsage::StreamDraw: return GL_STREAM_DRAW;
            case BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(const u16 *indices, u32 count) : IndexBuffer(IndexFormat::UInt16, count) {
        glCreateBuffers(1, &m_index_buffer_id);
        glNamedBufferData(m_index_buffer_id, count * sizeof(u16), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(const u32 *indices, u32 count) : IndexBuffer(IndexFormat::UInt32, count) {
        glCreateBuffers(1, &m_index_buffer_id);
        glNamedBufferData(m_index_buffer_id, count * sizeof(u32), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        glDeleteBuffers(1, &m_index_buffer_id);
    }

    void OpenGLIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_id);
    }

    void OpenGLIndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}