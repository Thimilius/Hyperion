#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class ISerializable;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class ISerializationStream {
    public:
        virtual void WriteInt8(const char *key, int8 value) = 0;
        virtual void WriteInt16(const char *key, int16 value) = 0;
        virtual void WriteInt32(const char *key, int32 value) = 0;
        virtual void WriteUInt8(const char *key, uint8 value) = 0;
        virtual void WriteUInt16(const char *key, uint16 value) = 0;
        virtual void WriteUInt32(const char *key, uint32 value) = 0;
        virtual void WriteString(const char *key, const String &value) = 0;
        virtual void WriteObject(const char *key, ISerializable *value) = 0;
        virtual void WriteStruct(const char *key, ISerializable *value) = 0;

        template<typename T>
        void WriteStruct(const char *key, T &value) {
            WriteStruct(key, static_cast<ISerializable *>(&value));
        }
    };

    class IDeserializationStream {
    public:
        using ObjectAllocatorFunction = std::function<ISerializable *()>;
    public:
        virtual int8 ReadInt8(const char *key) = 0;
        virtual int16 ReadInt16(const char *key) = 0;
        virtual int32 ReadInt32(const char *key) = 0;
        virtual uint8 ReadUInt8(const char *key) = 0;
        virtual uint16 ReadUInt16(const char *key) = 0;
        virtual uint32 ReadUInt32(const char *key) = 0;
        virtual String ReadString(const char *key) = 0;
        virtual void *ReadObject(const char *key, ObjectAllocatorFunction allocator) = 0;
        virtual void ReadStruct(const char *key, ISerializable *serializable) = 0;

        template<typename T>
        T *ReadObject(const char *key) {
            ObjectAllocatorFunction allocator = []() { return new T(); };
            return reinterpret_cast<T *>(ReadObject(key, allocator));
        }
        template<typename T>
        T *ReadObject(const char *key, ObjectAllocatorFunction allocator) {
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