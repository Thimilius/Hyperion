#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_vertex_array.hpp"

namespace Hyperion::Rendering {

    COpenGLVertexArray::COpenGLVertexArray() {
        glGenVertexArrays(1, &m_vertex_array_id);
    }

    COpenGLVertexArray::~COpenGLVertexArray() {
        glDeleteVertexArrays(1, &m_vertex_array_id);
    }

    void COpenGLVertexArray::Bind() const {
        glBindVertexArray(m_vertex_array_id);
    }

    void COpenGLVertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    void COpenGLVertexArray::AddVertexBuffer(const TRef<CVertexBuffer> &vertex_buffer) {
        Bind();

        vertex_buffer->Bind();

        const auto &layout = vertex_buffer->GetLayout();
        const auto &elements = layout.GetElements();
        for (s32 index = 0; index < elements.size(); index++) {
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

        m_vertex_buffers.push_back(vertex_buffer);
    }

    void COpenGLVertexArray::SetIndexBuffer(const TRef<CIndexBuffer> &index_buffer) {
        m_index_buffer = index_buffer;
        glVertexArrayElementBuffer(m_vertex_array_id, index_buffer->GetID());
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