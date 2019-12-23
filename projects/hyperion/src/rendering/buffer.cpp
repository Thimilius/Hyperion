#include "hyppch.hpp"

#include "hyperion/rendering/buffer.hpp"

#include "hyperion/driver/opengl/opengl_buffer.hpp"

namespace Hyperion::Rendering {

    u32 BufferElement::GetComponentCount() const {
        switch (type) {
            case ShaderDataType::Float:  return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3:   return 3 * 3;
            case ShaderDataType::Mat4:   return 4 * 4;
            case ShaderDataType::Int:    return 1;
            case ShaderDataType::Int2:   return 2;
            case ShaderDataType::Int3:   return 3;
            case ShaderDataType::Int4:   return 4;
            case ShaderDataType::Bool:   return 1;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    void BufferLayout::CalculateOffsetsAndStride() {
        uint32_t offset = 0;
        m_stride = 0;
        for (auto &element : m_elements) {
            element.offset = offset;
            offset += element.size;
            m_stride += element.size;
        }
    }

    Ref<VertexBuffer> VertexBuffer::Create(const u8 *vertices, u32 size) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Ref<VertexBuffer> VertexBuffer::Create(const u8 *vertices, u32 size, BufferUsage usage) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size, usage);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Ref<IndexBuffer> IndexBuffer::Create(const u16 *indices, u32 count) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, count);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Ref<IndexBuffer> IndexBuffer::Create(const u32 *indices, u32 count) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, count);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}