#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    enum class EShaderDataType {
        None,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool
    };

    static u32 ShaderDataTypeSize(EShaderDataType type) {
        switch (type) {
            case EShaderDataType::Float:  return 4;
            case EShaderDataType::Float2: return 4 * 2;
            case EShaderDataType::Float3: return 4 * 3;
            case EShaderDataType::Float4: return 4 * 4;
            case EShaderDataType::Mat3:   return 4 * 3 * 3;
            case EShaderDataType::Mat4:   return 4 * 4 * 4;
            case EShaderDataType::Int:    return 4;
            case EShaderDataType::Int2:   return 4 * 2;
            case EShaderDataType::Int3:   return 4 * 3;
            case EShaderDataType::Int4:   return 4 * 4;
            case EShaderDataType::Bool:   return 1;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    struct SBufferElement {
        CString name;
        EShaderDataType type;
        u32 size;
        u32 offset;
        bool normalized;

        SBufferElement() {}

        SBufferElement(const CString &name, EShaderDataType type, bool normalized = false)
            : name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) {
        }

        u32 GetComponentCount() const;
    };

    class CBufferLayout {
    private:
        CList<SBufferElement> m_elements;
        u32 m_stride = 0;
    public:
        CBufferLayout() {}

        CBufferLayout(const std::initializer_list<SBufferElement> &elements)
            : m_elements(elements) {
            CalculateOffsetsAndStride();
        }

        inline uint32_t GetStride() const { return m_stride; }
        inline const CList<SBufferElement> &GetElements() const { return m_elements; }
    private:
        void CalculateOffsetsAndStride();
    };

    class CVertexBuffer {
    protected:
        CBufferLayout m_layout;
    public:
        virtual ~CVertexBuffer() = default;

        const CBufferLayout &GetLayout() const { return m_layout; }
        void SetLayout(const CBufferLayout &layout) { m_layout = layout; }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static CVertexBuffer *Create(u8 *vertices, u32 size);
    };

    class CIndexBuffer {
    protected:
        u32 m_count;
    public:
        CIndexBuffer(u32 count) : m_count(count) {}
        virtual ~CIndexBuffer() = default;

        u32 GetCount() const { return m_count; }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static CIndexBuffer *Create(u32 *indices, u32 size);
    };

}