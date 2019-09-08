#pragma once

#include "hyperion/rendering/buffer.hpp"

namespace Hyperion::Rendering {

    class CVertexArray {
    public:
        virtual ~CVertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const TRef<CVertexBuffer> &vertex_buffer) = 0;
        virtual void SetIndexBuffer(const TRef<CIndexBuffer> &index_buffer) = 0;

        virtual const TVector<TRef<CVertexBuffer>> &GetVertexBuffers() const = 0;
        virtual const TRef<CIndexBuffer> &GetIndexBuffer() const = 0;

        static TRef<CVertexArray> Create();
    };

}