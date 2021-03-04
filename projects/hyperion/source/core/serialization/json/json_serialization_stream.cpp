//--------------------- Definition Include ---------------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/serialization/json/json_serialization_stream.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/serialization/serializable.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    class JsonArrayWriter : public IArrayWriter {
    public:
        void WriteBool(bool value) override { WriteValue(value); }
        void WriteInt8(int8 value) override { WriteValue(value); }
        void WriteInt16(int16 value) override { WriteValue(value); }
        void WriteInt32(int32 value) override { WriteValue(value); }
        void WriteUInt8(uint8 value) override { WriteValue(value); }
        void WriteUInt16(uint16 value) override { WriteValue(value); }
        void WriteUInt32(uint32 value) override { WriteValue(value); }
        void WriteString(const String &value) override { WriteValue(value); }
        void WriteObject(ISerializable *value) override {
            if (value) {
                JsonSerializationStream inner_stream;
                value->Serialize(inner_stream);
                m_json.push_back(inner_stream.m_json);
            } else {
                m_json.push_back(nullptr);
            }
        }
        void WriteStruct(ISerializable *value) override {
            WriteObject(value);
        }
        void WriteArray(uint64 length, ArrayWriterCallback callback) override {
            HYP_ASSERT(callback);

            JsonArrayWriter writer;
            for (uint64 i = 0; i < length; i++) {
                callback(i, writer);
            }
            m_json.push_back(writer.m_json);
        }
    private:
        template<typename T>
        void WriteValue(const T &value) {
            m_json.push_back(value);
        }
    private:
        nlohmann::ordered_json m_json = nlohmann::ordered_json::array();
    private:
        friend class Hyperion::JsonSerializationStream;
    };

    //--------------------------------------------------------------
    void JsonSerializationStream::WriteObject(const char *key, ISerializable *value) {
        HYP_ASSERT(key);

        if (value) {
            JsonSerializationStream inner_stream;
            value->Serialize(inner_stream);
            m_json[key] = inner_stream.m_json;
        } else {
            m_json[key] = nullptr;
        }
    }

    //--------------------------------------------------------------
    void JsonSerializationStream::WriteStruct(const char *key, ISerializable *value) {
        WriteObject(key, value);
    }

    //--------------------------------------------------------------
    void JsonSerializationStream::WriteArray(const char *key, uint64 length, ArrayWriterCallback callback) {
        HYP_ASSERT(key);
        HYP_ASSERT(callback);

        JsonArrayWriter writer;
        for (uint64 i = 0; i < length; i++) {
            callback(i, writer);
        }
        m_json[key] = writer.m_json;
    }

    //--------------------------------------------------------------
    String JsonSerializationStream::ToString() const {
        return m_json.dump(4);
    }

    //--------------------------------------------------------------
    class JsonArrayReader : public IArrayReader {
    public:
        JsonArrayReader(nlohmann::ordered_json &json) : m_json(json) { }
    public:
        String ReadString() {
            if (m_json.is_string()) {
                return m_json.get<String>();
            } else {
                return String();
            }
        }
    private:

    private:
        nlohmann::ordered_json &m_json;
    };

    //--------------------------------------------------------------
    bool JsonDeserializationStream::ReadBool(const char *key) {
        HYP_ASSERT(key);

        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_boolean()) {
            return json_key.get<bool>();
        } else {
            return false;
        }
    }

    //--------------------------------------------------------------
    String JsonDeserializationStream::ReadString(const char *key) {
        HYP_ASSERT(key);

        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_string()) {
            return json_key.get<String>();
        } else {
            return String();
        }
    }

    //--------------------------------------------------------------
    void *JsonDeserializationStream::ReadObject(const char *key, SerializableAllocatorFunction allocator) {
        HYP_ASSERT(key);
        HYP_ASSERT(allocator);

        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_object()) {
            ISerializable *serializable = allocator();

            JsonDeserializationStream inner_stream;
            inner_stream.m_json = json_key;
            serializable->Deserialize(inner_stream);
            return serializable;
        } else if (json_key.is_null()) {
            return nullptr;
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    void JsonDeserializationStream::ReadStruct(const char *key, ISerializable *serializable) {
        HYP_ASSERT(key);
        HYP_ASSERT(serializable);

        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_object()) {
            JsonDeserializationStream inner_stream;
            inner_stream.m_json = json_key;
            serializable->Deserialize(inner_stream);
        }
    }

    //--------------------------------------------------------------
    void JsonDeserializationStream::ReadArray(const char *key, ArrayReaderCallback callback) {
        HYP_ASSERT(key);
        HYP_ASSERT(callback);

        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_array()) {
            uint64 index = 0;
            for (nlohmann::ordered_json &element : json_key) {
                JsonArrayReader reader = JsonArrayReader(element);
                callback(index, reader);
            }
        }
    }

    //--------------------------------------------------------------
    void JsonDeserializationStream::Parse(const String &json_text) {
        m_json = nlohmann::ordered_json::parse(json_text, nullptr, false);
    }

}