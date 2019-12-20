#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_vertex_array.hpp"

namespace Hyperion::Rendering {

    OpenGLVertexArray::OpenGLVertexArray() {
        glGenVertexArrays(1, &m_vertex_array_id);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        glDeleteVertexArrays(1, &m_vertex_array_id);
    }

    void OpenGLVertexArray::Bind() const {
        glBindVertexArray(m_vertex_array_id);
    }

    void OpenGLVertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertex_buffer) {
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

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &index_buffer) {
        m_index_buffer = index_buffer;
        glVertexArrayElementBuffer(m_vertex_array_id, index_buffer->GetID());
    }

    u32 OpenGLVertexArray::ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:  return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3:   return GL_FLOAT;
            case ShaderDataType::Mat4:   return GL_FLOAT;
            case ShaderDataType::Int:    return GL_INT;
            case ShaderDataType::Int2:   return GL_INT;
            case ShaderDataType::Int3:   return GL_INT;
            case ShaderDataType::Int4:   return GL_INT;
            case ShaderDataType::Bool:   return GL_BOOL;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }
}