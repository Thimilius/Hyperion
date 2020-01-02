#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    enum class ShaderDataType {
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool
    };

    enum class IndexFormat {
        UInt16,
        UInt32
    };

    enum class BufferUsage {
        StaticDraw,
        StreamDraw,
        DynamicDraw
    };

    static u32 ShaderDataTypeSize(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:  return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Mat3:   return 4 * 3 * 3;
            case ShaderDataType::Mat4:   return 4 * 4 * 4;
            case ShaderDataType::Int:    return 4;
            case ShaderDataType::Int2:   return 4 * 2;
            case ShaderDataType::Int3:   return 4 * 3;
            case ShaderDataType::Int4:   return 4 * 4;
            case ShaderDataType::Bool:   return 1;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    struct BufferElement {
        String name;
        ShaderDataType type;
        u32 size;
        u32 offset;
        bool normalized;

        BufferElement() {}

        BufferElement(const String &name, ShaderDataType type, bool normalized = false)
            : name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) {
        }

        u32 GetComponentCount() const;
    };

    class BufferLayout {
    private:
        Vector<BufferElement> m_elements;
        u32 m_stride = 0;
    public:
        BufferLayout() {}

        BufferLayout(const std::initializer_list<BufferElement> &elements)
            : m_elements(elements) {
            CalculateOffsetsAndStride();
        }

        inline uint32_t GetStride() const { return m_stride; }
        inline const Vector<BufferElement> &GetElements() const { return m_elements; }
    private:
        void CalculateOffsetsAndStride();
    };

    class VertexBuffer {
    protected:
        u32 m_size;
        BufferLayout m_layout;
    public:
        VertexBuffer(u32 size) : m_size(size) { }
        virtual ~VertexBuffer() = default;

        inline const u32 GetSize() const { return m_size; }

        inline const BufferLayout &GetLayout() const { return m_layout; }
        inline void SetLayout(const BufferLayout &layout) { m_layout = layout; }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual u32 GetID() const = 0;

        virtual void SetData(u32 offset, u32 size, const u8 *verticies) = 0;

        static Ref<VertexBuffer> Create(const u8 *vertices, u32 size);
        static Ref<VertexBuffer> Create(const u8 *vertices, u32 size, BufferUsage usage);
    };

    class IndexBuffer {
    protected:
        IndexFormat m_format;
        u32 m_count;
    public:
        IndexBuffer(IndexFormat format, u32 count) : m_format(format), m_count(count) { }
        virtual ~IndexBuffer() = default;

        inline IndexFormat GetFormat() const { return m_format; }
        inline u32 GetCount() const { return m_count; }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual u32 GetID() const = 0;

        static Ref<IndexBuffer> Create(const u16 *indices, u32 count);
        static Ref<IndexBuffer> Create(const u32 *indices, u32 count);
    };

}