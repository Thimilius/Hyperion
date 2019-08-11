#pragma once

#include "hyperion/rendering/vertex_array.hpp"

namespace Hyperion::Rendering {

    class COpenGLVertexArray : public CVertexArray {
    private:
        u32 m_array_id;

        CList<PRef<CVertexBuffer>> m_vertex_buffers;
        PRef<CIndexBuffer> m_index_buffer;
    public:
        COpenGLVertexArray();
        ~COpenGLVertexArray();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const PRef<CVertexBuffer> &vertex_buffer) override;
        virtual void SetIndexBuffer(const PRef<CIndexBuffer> &index_buffer) override;

        virtual const CList<PRef<CVertexBuffer>> &GetVertexBuffers() const { return m_vertex_buffers; }
        virtual const PRef<CIndexBuffer> &GetIndexBuffer() const { return m_index_buffer; }

        static u32 ShaderDataTypeToOpenGLBaseType(EShaderDataType type);
    };

}