#pragma once

#include "hyperion/rendering/buffer.hpp"

namespace Hyperion::Rendering {

    class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual u32 GetID() const = 0;

        virtual void AddVertexBuffer(VertexBuffer *vertex_buffer) = 0;
        virtual void SetIndexBuffer(IndexBuffer *index_buffer) = 0;

        virtual const Vector<VertexBuffer *> &GetVertexBuffers() const = 0;
        virtual const IndexBuffer *GetIndexBuffer() const = 0;

        static VertexArray *Create();
    };

}