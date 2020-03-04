#include "hyppch.hpp"

#include "hyperion/rendering/buffer.hpp"

#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/driver/opengl/opengl_buffer.hpp"

namespace Hyperion::Rendering {

    u32 BufferElement::GetSize() const {
        switch (type) {
            case BufferDataType::Float:  return 4;
            case BufferDataType::Float2: return 4 * 2;
            case BufferDataType::Float3: return 4 * 3;
            case BufferDataType::Float4: return 4 * 4;
            case BufferDataType::Mat3:   return 4 * 3 * 3;
            case BufferDataType::Mat4:   return 4 * 4 * 4;
            case BufferDataType::Int:    return 4;
            case BufferDataType::Int2:   return 4 * 2;
            case BufferDataType::Int3:   return 4 * 3;
            case BufferDataType::Int4:   return 4 * 4;
            case BufferDataType::Bool:   return 1;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }

        return u32();
    }

    u32 BufferElement::GetComponentCount() const {
        switch (type) {
            case BufferDataType::Float:  return 1;
            case BufferDataType::Float2: return 2;
            case BufferDataType::Float3: return 3;
            case BufferDataType::Float4: return 4;
            case BufferDataType::Mat3:   return 3 * 3;
            case BufferDataType::Mat4:   return 4 * 4;
            case BufferDataType::Int:    return 1;
            case BufferDataType::Int2:   return 2;
            case BufferDataType::Int3:   return 3;
            case BufferDataType::Int4:   return 4;
            case BufferDataType::Bool:   return 1;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    void BufferLayout::CalculateOffsetsAndStride() {
        uint32_t offset = 0;
        m_stride = 0;
        for (auto &element : m_elements) {
            element.offset = offset;
            offset += element.GetSize();
            m_stride += element.GetSize();
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