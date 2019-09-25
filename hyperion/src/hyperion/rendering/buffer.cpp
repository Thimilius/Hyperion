#include "hyppch.hpp"

#include "hyperion/rendering/buffer.hpp"

#include "hyperion/platform/opengl/opengl_buffer.hpp"

namespace Hyperion::Rendering {

    u32 SBufferElement::GetComponentCount() const {
        switch (type) {
            case EShaderDataType::Float:  return 1;
            case EShaderDataType::Float2: return 2;
            case EShaderDataType::Float3: return 3;
            case EShaderDataType::Float4: return 4;
            case EShaderDataType::Mat3:   return 3 * 3;
            case EShaderDataType::Mat4:   return 4 * 4;
            case EShaderDataType::Int:    return 1;
            case EShaderDataType::Int2:   return 2;
            case EShaderDataType::Int3:   return 3;
            case EShaderDataType::Int4:   return 4;
            case EShaderDataType::Bool:   return 1;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    void CBufferLayout::CalculateOffsetsAndStride() {
        uint32_t offset = 0;
        m_stride = 0;
        for (auto &element : m_elements) {
            element.offset = offset;
            offset += element.size;
            m_stride += element.size;
        }
    }

    TRef<CVertexBuffer> CVertexBuffer::Create(u8 *vertices, u32 size) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return std::make_shared<COpenGLVertexBuffer>(vertices, size);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    TRef<CIndexBuffer> CIndexBuffer::Create(u16 *indices, u32 count) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return std::make_shared<COpenGLIndexBuffer>(indices, count);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    TRef<CIndexBuffer> CIndexBuffer::Create(u32 *indices, u32 count) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return std::make_shared<COpenGLIndexBuffer>(indices, count);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}