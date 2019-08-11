#pragma once

#include "hyperion/rendering/buffer.hpp"

namespace Hyperion::Rendering {

    class COpenGLVertexBuffer : public CVertexBuffer {
    private:
        u32 m_buffer_id;
    public:
        COpenGLVertexBuffer(u8 *vertices, u32 size);
        ~COpenGLVertexBuffer();

        void Bind() const override;
        void Unbind() const override;
    };

    class COpenGLIndexBuffer : public CIndexBuffer {
    private:
        u32 m_buffer_id;
    public:
        COpenGLIndexBuffer(u32 *indices, u32 count);
        ~COpenGLIndexBuffer();

        void Bind() const override;
        void Unbind() const override;
    };

}