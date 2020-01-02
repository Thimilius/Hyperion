#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_vertex_array.hpp"

#include <glad/glad.h>

namespace Hyperion::Rendering {

    OpenGLVertexArray::OpenGLVertexArray() {
        glCreateVertexArrays(1, &m_vertex_array_id);
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
        const auto &layout = vertex_buffer->GetLayout();
        const auto &elements = layout.GetElements();
        for (s32 index = 0; index < elements.size(); index++) {
            auto &element = elements[index];

            // To better distinguish the different indicies used
            s32 attribute_index = index;
            s32 binding_index = (s32)m_vertex_buffers.size();

            // This is the modern way of defining the format of a vertex buffer in OpenGL using DSA
            GLboolean normalized = element.normalized ? GL_TRUE : GL_FALSE;
            glEnableVertexArrayAttrib(m_vertex_array_id, attribute_index);
            glVertexArrayAttribFormat(m_vertex_array_id, attribute_index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type), normalized, element.offset);
            glVertexArrayVertexBuffer(m_vertex_array_id, binding_index, vertex_buffer->GetID(), 0, layout.GetStride());
            glVertexArrayAttribBinding(m_vertex_array_id, attribute_index, binding_index);
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