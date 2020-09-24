#pragma once

#include "hyperion/rendering/vertex_array.hpp"

namespace Hyperion::Rendering {

    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual u32 GetID() const override { return m_vertex_array_id; }

        virtual void AddVertexBuffer(VertexBuffer *vertex_buffer) override;
        virtual void SetIndexBuffer(IndexBuffer *index_buffer) override;

        virtual const Vector<VertexBuffer *> &GetVertexBuffers() const { return m_vertex_buffers; }
        virtual const IndexBuffer *GetIndexBuffer() const { return m_index_buffer; }

        static u32 BufferDataTypeToOpenGLBaseType(BufferDataType type);
    private:
        u32 m_vertex_array_id;

        Vector<VertexBuffer *> m_vertex_buffers;
        IndexBuffer *m_index_buffer;
    };

}