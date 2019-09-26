#pragma once

#include "hyperion/rendering/vertex_array.hpp"

namespace Hyperion::Rendering {

    class COpenGLVertexArray : public CVertexArray {
    private:
        u32 m_vertex_array_id;

        TVector<TRef<CVertexBuffer>> m_vertex_buffers;
        TRef<CIndexBuffer> m_index_buffer;
    public:
        COpenGLVertexArray();
        ~COpenGLVertexArray();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual u32 GetID() const override { return m_vertex_array_id; }

        virtual void AddVertexBuffer(const TRef<CVertexBuffer> &vertex_buffer) override;
        virtual void SetIndexBuffer(const TRef<CIndexBuffer> &index_buffer) override;

        virtual const TVector<TRef<CVertexBuffer>> &GetVertexBuffers() const { return m_vertex_buffers; }
        virtual const TRef<CIndexBuffer> &GetIndexBuffer() const { return m_index_buffer; }

        static u32 ShaderDataTypeToOpenGLBaseType(EShaderDataType type);
    };

}