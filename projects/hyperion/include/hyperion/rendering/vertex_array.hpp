#pragma once

#include "hyperion/rendering/buffer.hpp"

namespace Hyperion::Rendering {

    class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual u32 GetID() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertex_buffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer> &index_buffer) = 0;

        virtual const Vector<Ref<VertexBuffer>> &GetVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer> &GetIndexBuffer() const = 0;

        static Ref<VertexArray> Create();
    };

}