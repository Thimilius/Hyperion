#pragma once

#include "hyperion/rendering/buffer.hpp"

namespace Hyperion::Rendering {

    class OpenGLVertexBuffer : public VertexBuffer {
    private:
        u32 m_vertex_buffer_id;
    public:
        OpenGLVertexBuffer(const u8 *vertices, u32 size);
        OpenGLVertexBuffer(const u8 *vertices, u32 size, BufferUsage usage);
        ~OpenGLVertexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        u32 GetID() const override { return m_vertex_buffer_id; }

        void SetData(u32 offset, const u8 *verticies, u32 size) override;
    private:
        u32 GetGLBufferUsage(BufferUsage usage);
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    private:
        u32 m_index_buffer_id;
    public:
        OpenGLIndexBuffer(const u16 *indices, u32 count);
        OpenGLIndexBuffer(const u32 *indices, u32 count);
        ~OpenGLIndexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        u32 GetID() const override { return m_index_buffer_id; }
    };

}