#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/vec4.hpp"
#include "hyperion/core/math/quaternion.hpp"
#include "hyperion/core/serialization/reference_context.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class IArrayReader;
    class IArrayWriter;
    class ISerializable;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using ArrayWriterCallback = std::function<void(uint64, IArrayWriter &)>;

    class IArrayWriter {
    public:
        virtual ~IArrayWriter() = default;
    public:
        virtual void WriteBool(bool value) = 0;
        virtual void WriteInt8(int8 value) = 0;
        virtual void WriteInt16(int16 value) = 0;
        virtual void WriteInt32(int32 value) = 0;
        virtual void WriteUInt8(uint8 value) = 0;
        virtual void WriteUInt16(uint16 value) = 0;
        virtual void WriteUInt32(uint32 value) = 0;
        virtual void WriteFloat(float32 value) = 0;
        virtual void WriteString(const String &value) = 0;
        virtual void WriteColor(Color value) = 0;
        virtual void WriteVec2(Vec2 value) = 0;
        virtual void WriteVec3(Vec3 value) = 0;
        virtual void WriteVec4(Vec4 value) = 0;
        virtual void WriteQuaternion(Quaternion value) = 0;
        virtual void WriteObject(ISerializable *value) = 0;
        virtual void WriteStruct(ISerializable *value) = 0;
        virtual void WriteArray(uint64 length, ArrayWriterCallback callback) = 0;

        template<typename T>
        void WriteStruct(T &value) {
            WriteStruct(static_cast<ISerializable *>(&value));
        }
    };

    class ISerializationStream {
    public:
        virtual ~ISerializationStream() = default;
    public:
        virtual void WriteBool(const char *key, bool value) = 0;
        virtual void WriteInt8(const char *key, int8 value) = 0;
        virtual void WriteInt16(const char *key, int16 value) = 0;
        virtual void WriteInt32(const char *key, int32 value) = 0;
        virtual void WriteUInt8(const char *key, uint8 value) = 0;
        virtual void WriteUInt16(const char *key, uint16 value) = 0;
        virtual void WriteUInt32(const char *key, uint32 value) = 0;
        virtual void WriteFloat(const char *key, float32 value) = 0;
        virtual void WriteString(const char *key, const String &value) = 0;
        virtual void WriteColor(const char *key, Color value) = 0;
        virtual void WriteVec2(const char *key, Vec2 value) = 0;
        virtual void WriteVec3(const char *key, Vec3 value) = 0;
        virtual void WriteVec4(const char *key, Vec4 value) = 0;
        virtual void WriteQuaternion(const char *key, Quaternion value) = 0;
        virtual void WriteObject(const char *key, ISerializable *value) = 0;
        virtual void WriteStruct(const char *key, ISerializable *value) = 0;
        virtual void WriteArray(const char *key, uint64 length, ArrayWriterCallback callback) = 0;

        template<typename T>
        void WriteStruct(const char *key, T &value) {
            WriteStruct(key, static_cast<ISerializable *>(&value));
        }
    };

    using SerializableAllocatorFunction = std::function<ISerializable *()>;

    using ArrayReaderCallback = std::function<void(uint64, IArrayReader &)>;

    class IArrayReader {
    public:
        virtual ~IArrayReader() = default;
    public:
        virtual bool ReadBool() = 0;
        virtual int8 ReadInt8() = 0;
        virtual int16 ReadInt16() = 0;
        virtual int32 ReadInt32() = 0;
        virtual uint8 ReadUInt8() = 0;
        virtual uint16 ReadUInt16() = 0;
        virtual uint32 ReadUInt32() = 0;
        virtual float32 ReadFloat() = 0;
        virtual String ReadString() = 0;
        virtual Color ReadColor() = 0;
        virtual Vec2 ReadVec2() = 0;
        virtual Vec3 ReadVec3() = 0;
        virtual Vec4 ReadVec4() = 0;
        virtual Quaternion ReadQuaternion() = 0;
        virtual void *ReadObject(ReferenceContext &context, SerializableAllocatorFunction allocator) = 0;
        virtual void ReadStruct(ReferenceContext &context, ISerializable *serializable) = 0;
        virtual void ReadArray(ReferenceContext &context, ArrayReaderCallback callback) = 0;

        template<typename T>
        T *ReadObject(ReferenceContext &context) {
            SerializableAllocatorFunction allocator = []() { return new T(); };
            return reinterpret_cast<T *>(ReadObject(context, allocator));
        }
        template<typename T>
        T *ReadObject(ReferenceContext &context, SerializableAllocatorFunction allocator) {
            return reinterpret_cast<T *>(ReadObject(context, allocator));
        }

        template<typename T>
        T ReadStruct(ReferenceContext &context) {
            T object = { };
            ReadStruct(context, static_cast<ISerializable *>(&object));
            return object;
        }
    };

    class IDeserializationStream {
    public:
    public:
        virtual ~IDeserializationStream() = default;
    public:
        virtual bool ReadBool(const char *key) = 0;
        virtual int8 ReadInt8(const char *key) = 0;
        virtual int16 ReadInt16(const char *key) = 0;
        virtual int32 ReadInt32(const char *key) = 0;
        virtual uint8 ReadUInt8(const char *key) = 0;
        virtual uint16 ReadUInt16(const char *key) = 0;
        virtual uint32 ReadUInt32(const char *key) = 0;
        virtual float32 ReadFloat(const char *key) = 0;
        virtual String ReadString(const char *key) = 0;
        virtual Color ReadColor(const char *key) = 0;
        virtual Vec2 ReadVec2(const char *key) = 0;
        virtual Vec3 ReadVec3(const char *key) = 0;
        virtual Vec4 ReadVec4(const char *key) = 0;
        virtual Quaternion ReadQuaternion(const char *key) = 0;
        virtual void *ReadObject(const char *key, ReferenceContext &context, SerializableAllocatorFunction allocator) = 0;
        virtual void ReadStruct(const char *key, ReferenceContext &context, ISerializable *serializable) = 0;
        virtual void ReadArray(const char *key, ReferenceContext &context, ArrayReaderCallback callback) = 0;

        template<typename T>
        T *ReadObject(const char *key, ReferenceContext &context) {
            SerializableAllocatorFunction allocator = []() { return new T(); };
            return reinterpret_cast<T *>(ReadObject(key, context, allocator));
        }
        template<typename T>
        T *ReadObject(const char *key, ReferenceContext &context, SerializableAllocatorFunction allocator) {
            return reinterpret_cast<T *>(ReadObject(key, context, allocator));
        }

        template<typename T>
        T ReadStruct(const char *key, ReferenceContext &context) {
            T object = { };
            ReadStruct(key, context, static_cast<ISerializable *>(&object));
            return object;
        }
    };

}