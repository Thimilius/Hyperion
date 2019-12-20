#pragma once

#include "hyperion/rendering/buffer.hpp"

namespace Hyperion::Rendering {

    class COpenGLVertexBuffer : public CVertexBuffer {
    private:
        u32 m_vertex_buffer_id;
    public:
        COpenGLVertexBuffer(const u8 *vertices, u32 size);
        COpenGLVertexBuffer(const u8 *vertices, u32 size, EBufferUsage usage);
        ~COpenGLVertexBuffer();

        void Bind() const override;
        void Unbind() const override;

        u32 GetID() const override { return m_vertex_buffer_id; }

        void SetData(u32 offset, const u8 *verticies, u32 size) override;
    private:
        u32 GetGLBufferUsage(EBufferUsage usage);
    };

    class COpenGLIndexBuffer : public CIndexBuffer {
    private:
        u32 m_index_buffer_id;
    public:
        COpenGLIndexBuffer(const u16 *indices, u32 count);
        COpenGLIndexBuffer(const u32 *indices, u32 count);
        ~COpenGLIndexBuffer();

        void Bind() const override;
        void Unbind() const override;

        u32 GetID() const override { return m_index_buffer_id; }
    };

}