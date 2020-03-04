#pragma once

#include "hyperion/rendering/vertex_array.hpp"

namespace Hyperion::Rendering {

    class OpenGLVertexArray : public VertexArray {
    private:
        u32 m_vertex_array_id;

        Vector<Ref<VertexBuffer>> m_vertex_buffers;
        Ref<IndexBuffer> m_index_buffer;
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual u32 GetID() const override { return m_vertex_array_id; }

        virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertex_buffer) override;
        virtual void SetIndexBuffer(const Ref<IndexBuffer> &index_buffer) override;

        virtual const Vector<Ref<VertexBuffer>> &GetVertexBuffers() const { return m_vertex_buffers; }
        virtual const Ref<IndexBuffer> &GetIndexBuffer() const { return m_index_buffer; }

        static u32 BufferDataTypeToOpenGLBaseType(BufferDataType type);
    };

}