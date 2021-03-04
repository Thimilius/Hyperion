#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

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
        virtual void WriteString(const String &value) = 0;
        virtual void WriteObject(ISerializable *value) = 0;
        virtual void WriteStruct(ISerializable *value) = 0;
        virtual void WriteArray(uint64 length, ArrayWriterCallback callback) = 0;
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
        virtual void WriteString(const char *key, const String &value) = 0;
        virtual void WriteObject(const char *key, ISerializable *value) = 0;
        virtual void WriteStruct(const char *key, ISerializable *value) = 0;
        virtual void WriteArray(const char *key, uint64 length, ArrayWriterCallback callback) = 0;

        template<typename T>
        void WriteStruct(const char *key, T &value) {
            WriteStruct(key, static_cast<ISerializable *>(&value));
        }
    };

    using ArrayReaderCallback = std::function<void(uint64, IArrayReader &)>;

    class IArrayReader {
    public:
        virtual ~IArrayReader() = default;
    public:
        virtual String ReadString() = 0;
    };

    using SerializableAllocatorFunction = std::function<ISerializable *()>;

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
        virtual String ReadString(const char *key) = 0;
        virtual void *ReadObject(const char *key, SerializableAllocatorFunction allocator) = 0;
        virtual void ReadStruct(const char *key, ISerializable *serializable) = 0;
        virtual void ReadArray(const char *key, ArrayReaderCallback callback) = 0;

        template<typename T>
        T *ReadObject(const char *key) {
            SerializableAllocatorFunction allocator = []() { return new T(); };
            return reinterpret_cast<T *>(ReadObject(key, allocator));
        }
        template<typename T>
        T *ReadObject(const char *key, SerializableAllocatorFunction allocator) {
            return reinterpret_cast<T *>(ReadObject(key, allocator));
        }

        template<typename T>
        T ReadStruct(const char *key) {
            T object = { };
            ReadStruct(key, static_cast<ISerializable *>(&object));
            return object;
        }
    };

}