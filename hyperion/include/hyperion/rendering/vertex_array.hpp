#pragma once

#include "hyperion/rendering/buffer.hpp"

namespace Hyperion::Rendering {

    class CVertexArray {
    public:
        virtual ~CVertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const PRef<CVertexBuffer> &vertex_buffer) = 0;
        virtual void SetIndexBuffer(const PRef<CIndexBuffer> &index_buffer) = 0;

        virtual const CList<PRef<CVertexBuffer>> &GetVertexBuffers() const = 0;
        virtual const PRef<CIndexBuffer> &GetIndexBuffer() const = 0;

        static CVertexArray *Create();
    };

}