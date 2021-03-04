//--------------------- Definition Include ---------------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/serialization/json/json_serialization_stream.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/serialization/serializable.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

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
    String JsonSerializationStream::ToString() const {
        return m_json.dump(4);
    }

    //--------------------------------------------------------------
    String JsonDeserializationStream::ReadString(const char *key) {
        HYP_ASSERT(key);

        nlohmann::ordered_json json_key = m_json[key];
        if (json_key.is_string()) {
            return json_key.get<String>();
        } else {
            return String();
        }
    }

    //--------------------------------------------------------------
    void *JsonDeserializationStream::ReadObject(const char *key, ObjectAllocatorFunction allocator) {
        HYP_ASSERT(key);
        HYP_ASSERT(allocator);

        nlohmann::ordered_json json_key = m_json[key];
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

        nlohmann::ordered_json json_key = m_json[key];
        if (json_key.is_object()) {
            JsonDeserializationStream inner_stream;
            inner_stream.m_json = json_key;
            serializable->Deserialize(inner_stream);
        }
    }

    //--------------------------------------------------------------
    void JsonDeserializationStream::Parse(const String &json_text) {
        m_json = nlohmann::ordered_json::parse(json_text, nullptr, false);
    }

}