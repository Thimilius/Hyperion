#pragma once

//---------------------- Library Includes ----------------------
#include <nlohmann/json.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/core/serialization/serialization_stream.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class JsonArrayReader;
    class JsonArrayWriter;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class JsonSerializationStream : public ISerializationStream {
    public:
        void WriteBool(const char *key, bool value) override { WriteValue(key, value); }
        void WriteInt8(const char *key, int8 value) override { WriteValue(key, value); }
        void WriteInt16(const char *key, int16 value) override { WriteValue(key, value); }
        void WriteInt32(const char *key, int32 value) override { WriteValue(key, value); }
        void WriteUInt8(const char *key, uint8 value) override { WriteValue(key, value); }
        void WriteUInt16(const char *key, uint16 value) override { WriteValue(key, value); }
        void WriteUInt32(const char *key, uint32 value) override { WriteValue(key, value); }
        void WriteFloat(const char *key, float32 value) override { WriteValue(key, value); }
        void WriteString(const char *key, const String &value) override { WriteValue(key, value); }
        void WriteColor(const char *key, Color value) override;
        void WriteVec2(const char *key, Vec2 value) override;
        void WriteVec3(const char *key, Vec3 value) override;
        void WriteVec4(const char *key, Vec4 value) override;
        void WriteQuaternion(const char *key, Quaternion value) override;
        void WriteObject(const char *key, ISerializable *value) override;
        void WriteStruct(const char *key, ISerializable *value) override;
        void WriteArray(const char *key, uint64 length, ArrayWriterCallback callback) override;

        String ToString() const;
    private:
        template<typename T>
        void WriteValue(const char *key, const T &value) {
            HYP_ASSERT(key);

            m_json[key] = value;
        }
    private:
        nlohmann::ordered_json m_json = nlohmann::ordered_json::object();
    private:
        friend class Hyperion::JsonArrayWriter;
    };

    class JsonDeserializationStream : public IDeserializationStream {
    public:
        bool ReadBool(const char *key) override;
        int8 ReadInt8(const char *key) override { return ReadInt<int8>(key); }
        int16 ReadInt16(const char *key) override { return ReadInt<int16>(key); }
        int32 ReadInt32(const char *key) override { return ReadInt<int32>(key); }
        uint8 ReadUInt8(const char *key) override { return ReadUInt<uint8>(key); }
        uint16 ReadUInt16(const char *key) override { return ReadUInt<uint16>(key); }
        uint32 ReadUInt32(const char *key) override { return ReadUInt<uint32>(key); }
        float32 ReadFloat(const char *key) override;
        String ReadString(const char *key) override;
        Color ReadColor(const char *key) override;
        Vec2 ReadVec2(const char *key) override;
        Vec3 ReadVec3(const char *key) override;
        Vec4 ReadVec4(const char *key) override;
        Quaternion ReadQuaternion(const char *key) override;
        ISerializable *ReadObject(const char *key, SerializableAllocatorFunction allocator) override;
        void ReadStruct(const char *key, ISerializable *serializable) override;
        void ReadArray(const char *key, ArrayReaderCallback callback) override;

        void Parse(const String &json_text);
    private:
        template<typename T>
        T ReadInt(const char *key) {
            HYP_ASSERT(key);

            nlohmann::ordered_json json_key = m_json[key];
            if (json_key.is_number_integer()) {
                return json_key.get<T>();
            } else {
                return 0;
            }
        }

        template<typename T>
        T ReadUInt(const char *key) {
            HYP_ASSERT(key);

            nlohmann::ordered_json json_key = m_json[key];
            if (json_key.is_number_unsigned()) {
                return json_key.get<T>();
            } else {
                return 0;
            }
        }
    private:
        nlohmann::ordered_json m_json;
    private:
        friend class Hyperion::JsonArrayReader;
    };

}