#include "hyppch.hpp"

#include "opengl_vertex_array.hpp"

namespace Hyperion::Rendering {

    COpenGLVertexArray::COpenGLVertexArray() {
        glGenVertexArrays(1, &m_array_id);
    }

    COpenGLVertexArray::~COpenGLVertexArray() {
        glDeleteVertexArrays(1, &m_array_id);
    }

    void COpenGLVertexArray::Bind() const {
        glBindVertexArray(m_array_id);
    }

    void COpenGLVertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    void COpenGLVertexArray::AddVertexBuffer(const PRef<CVertexBuffer> &vertex_buffer) {
        Bind();

        vertex_buffer->Bind();

        const auto &layout = vertex_buffer->GetLayout();
        const auto &elements = layout.GetElements();
        for (s32 index = 0; index < elements.GetCount(); index++) {
            auto &element = elements[index];
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void *)(u64)element.offset);
        }

        m_vertex_buffers.Add(vertex_buffer);
    }

    void COpenGLVertexArray::SetIndexBuffer(const PRef<CIndexBuffer> &index_buffer) {
        Bind();

        index_buffer->Bind();
        m_index_buffer = index_buffer;
    }

    u32 COpenGLVertexArray::ShaderDataTypeToOpenGLBaseType(EShaderDataType type) {
        switch (type) {
            case EShaderDataType::Float:  return GL_FLOAT;
            case EShaderDataType::Float2: return GL_FLOAT;
            case EShaderDataType::Float3: return GL_FLOAT;
            case EShaderDataType::Float4: return GL_FLOAT;
            case EShaderDataType::Mat3:   return GL_FLOAT;
            case EShaderDataType::Mat4:   return GL_FLOAT;
            case EShaderDataType::Int:    return GL_INT;
            case EShaderDataType::Int2:   return GL_INT;
            case EShaderDataType::Int3:   return GL_INT;
            case EShaderDataType::Int4:   return GL_INT;
            case EShaderDataType::Bool:   return GL_BOOL;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }
}