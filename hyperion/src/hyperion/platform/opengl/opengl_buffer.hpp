#pragma once

#include "hyperion/rendering/buffer.hpp"

namespace Hyperion::Rendering {

    class COpenGLVertexBuffer : public CVertexBuffer {
    public:
        COpenGLVertexBuffer(u8 *vertices, u32 size);
        ~COpenGLVertexBuffer();

        void Bind() const override;
        void Unbind() const override;
    private:
        u32 m_buffer_id;
    };

    class COpenGLIndexBuffer : public CIndexBuffer {
    public:
        COpenGLIndexBuffer(u32 *indices, u32 count);
        ~COpenGLIndexBuffer();

        void Bind() const override;
        void Unbind() const override;
    private:
        u32 m_buffer_id;
    };

}