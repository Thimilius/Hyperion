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

    enum class EIndexFormat {
        None,
        UInt16,
        UInt32
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
        TString name;
        EShaderDataType type;
        u32 size;
        u32 offset;
        bool normalized;

        SBufferElement() {}

        SBufferElement(const TString &name, EShaderDataType type, bool normalized = false)
            : name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) {
        }

        u32 GetComponentCount() const;
    };

    class CBufferLayout {
    private:
        TVector<SBufferElement> m_elements;
        u32 m_stride = 0;
    public:
        CBufferLayout() {}

        CBufferLayout(const std::initializer_list<SBufferElement> &elements)
            : m_elements(elements) {
            CalculateOffsetsAndStride();
        }

        inline uint32_t GetStride() const { return m_stride; }
        inline const TVector<SBufferElement> &GetElements() const { return m_elements; }
    private:
        void CalculateOffsetsAndStride();
    };

    class CVertexBuffer {
    protected:
        CBufferLayout m_layout;
    public:
        virtual ~CVertexBuffer() = default;

        inline const CBufferLayout &GetLayout() const { return m_layout; }
        inline void SetLayout(const CBufferLayout &layout) { m_layout = layout; }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual u32 GetID() const = 0;

        static TRef<CVertexBuffer> Create(const u8 *vertices, u32 size);
    };

    class CIndexBuffer {
    protected:
        EIndexFormat m_format;
        u32 m_count;
    public:
        CIndexBuffer(EIndexFormat format, u32 count) : m_format(format), m_count(count) { }
        virtual ~CIndexBuffer() = default;

        inline EIndexFormat GetFormat() const { return m_format; }
        inline u32 GetCount() const { return m_count; }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual u32 GetID() const = 0;

        static TRef<CIndexBuffer> Create(const u16 *indices, u32 count);
        static TRef<CIndexBuffer> Create(const u32 *indices, u32 count);
    };

}