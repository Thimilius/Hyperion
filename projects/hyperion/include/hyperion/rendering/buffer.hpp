#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    enum class BufferDataType {
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

    struct BufferElement {
        String name;
        BufferDataType type;
        u32 offset;
        bool normalized;

        BufferElement() { }

        BufferElement(const String &name, BufferDataType type, bool normalized = false)
            : name(name), type(type), offset(0), normalized(normalized) {
        }

        u32 GetSize() const;
        u32 GetComponentCount() const;
    };

    class BufferLayout {
    public:
        BufferLayout() { }

        BufferLayout(const std::initializer_list<BufferElement> &elements)
            : m_elements(elements) {
            CalculateOffsetsAndStride();
        }

        inline uint32_t GetStride() const { return m_stride; }
        inline const Vector<BufferElement> &GetElements() const { return m_elements; }
    private:
        void CalculateOffsetsAndStride();
    private:
        Vector<BufferElement> m_elements;
        u32 m_stride = 0;
    };

    class VertexBuffer {
    public:
        VertexBuffer(u32 size) : m_size(size) { }
        virtual ~VertexBuffer() = default;

        inline u32 GetSize() const { return m_size; }

        inline const BufferLayout &GetLayout() const { return m_layout; }
        inline void SetLayout(const BufferLayout &layout) { m_layout = layout; }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual u32 GetID() const = 0;

        virtual void SetData(u32 offset, u32 size, const u8 *verticies) = 0;

        static VertexBuffer *Create(const u8 *vertices, u32 size);
        static VertexBuffer *Create(const u8 *vertices, u32 size, BufferUsage usage);
    protected:
        u32 m_size;
        BufferLayout m_layout;
    };

    class IndexBuffer {
    public:
        IndexBuffer(IndexFormat format, u32 count) : m_format(format), m_count(count) { }
        virtual ~IndexBuffer() = default;

        inline IndexFormat GetFormat() const { return m_format; }
        inline u32 GetCount() const { return m_count; }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual u32 GetID() const = 0;

        static IndexBuffer *Create(const u16 *indices, u32 count);
        static IndexBuffer *Create(const u32 *indices, u32 count);
    protected:
        IndexFormat m_format;
        u32 m_count;
    };

    class UniformBuffer {
    public:
        UniformBuffer(u32 size) : m_size(size) { }
        virtual ~UniformBuffer() = default;

        inline u32 GetSize() const { return m_size; }

        virtual void Bind(u32 binding_point) const = 0;
        virtual void Unbind(u32 binding_point) const = 0;

        virtual u32 GetID() const = 0;

        virtual void SetData(u32 offset, u32 size, const u8 *data) = 0;

        static UniformBuffer *Create(const u8 *data, u32 size);
        static UniformBuffer *Create(const u8 *data, u32 size, BufferUsage usage);
    protected:
        u32 m_size;
    };

}