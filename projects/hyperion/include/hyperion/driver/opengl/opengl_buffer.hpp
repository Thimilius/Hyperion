#pragma once

#include "hyperion/rendering/buffer.hpp"

namespace Hyperion::Rendering {

    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(const u8 *vertices, u32 size);
        OpenGLVertexBuffer(const u8 *vertices, u32 size, BufferUsage usage);
        ~OpenGLVertexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        u32 GetID() const override { return m_vertex_buffer_id; }

        void SetData(u32 offset, u32 size, const u8 *verticies) override;
    private:
        u32 m_vertex_buffer_id;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(const u16 *indices, u32 count);
        OpenGLIndexBuffer(const u32 *indices, u32 count);
        ~OpenGLIndexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        u32 GetID() const override { return m_index_buffer_id; }
    private:
        u32 m_index_buffer_id;
    };

    class OpenGLUniformBuffer : public UniformBuffer {
    public:
        OpenGLUniformBuffer(const u8 *data, u32 size);
        OpenGLUniformBuffer(const u8 *data, u32 size, BufferUsage usage);
        ~OpenGLUniformBuffer() override;

        void Bind(u32 binding_point) const override;
        void Unbind(u32 binding_point) const override;

        u32 GetID() const override { return m_uniform_buffer_id; }

        void SetData(u32 offset, u32 size, const u8 *data) override;
    private:
        u32 m_uniform_buffer_id;
    };

}