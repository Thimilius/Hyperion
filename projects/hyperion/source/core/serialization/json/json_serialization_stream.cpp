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
        void WriteFloat(float32 value) override { WriteValue(value); }
        void WriteString(const String &value) override { WriteValue(value); }
        void WriteColor(Color value) override {
            m_json = nlohmann::ordered_json::object({ { "r", value.r }, { "g", value.g }, { "b", value.b }, { "a", value.a } });
        }
        void WriteVec2(Vec2 value) override {
            m_json = nlohmann::ordered_json::object({ { "x", value.x }, { "y", value.y } });
        }
        void WriteVec3(Vec3 value) override {
            m_json = nlohmann::ordered_json::object({ { "x", value.x }, { "y", value.y }, { "z", value.z } });
        }
        void WriteVec4(Vec4 value) override {
            m_json = nlohmann::ordered_json::object({ { "x", value.x }, { "y", value.y }, { "z", value.z }, { "w", value.w } });
        }
        void WriteQuaternion(Quaternion value) override {
            m_json = nlohmann::ordered_json::object({ { "x", value.x }, { "y", value.y }, { "z", value.z }, { "w", value.w } });
        }
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
    void JsonSerializationStream::WriteColor(const char *key, Color value) {
        HYP_ASSERT(key);

        m_json[key] = nlohmann::ordered_json::object({ { "r", value.r }, { "g", value.g }, { "b", value.b }, { "a", value.a } });
    }

    //--------------------------------------------------------------
    void JsonSerializationStream::WriteVec2(const char *key, Vec2 value) {
        HYP_ASSERT(key);

        m_json[key] = nlohmann::ordered_json::object({ { "x", value.x }, { "y", value.y } });
    }

    //--------------------------------------------------------------
    void JsonSerializationStream::WriteVec3(const char *key, Vec3 value) {
        HYP_ASSERT(key);

        m_json[key] = nlohmann::ordered_json::object({ { "x", value.x }, { "y", value.y }, { "z", value.z } });
    }

    //--------------------------------------------------------------
    void JsonSerializationStream::WriteVec4(const char *key, Vec4 value) {
        HYP_ASSERT(key);

        m_json[key] = nlohmann::ordered_json::object({ { "x", value.x }, { "y", value.y }, { "z", value.z }, { "w", value.w } });
    }

    //--------------------------------------------------------------
    void JsonSerializationStream::WriteQuaternion(const char *key, Quaternion value) {
        HYP_ASSERT(key);

        m_json[key] = nlohmann::ordered_json::object({ { "x", value.x }, { "y", value.y }, { "z", value.z }, { "w", value.w } });
    }

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
        bool ReadBool() override {
            if (m_json.is_boolean()) {
                return m_json.get<bool>();
            } else {
                return false;
            }
        }
        int8 ReadInt8() override { return ReadInt<int8>(); }
        int16 ReadInt16() override { return ReadInt<int16>(); }
        int32 ReadInt32() override { return ReadInt<int32>(); }
        uint8 ReadUInt8() override { return ReadUInt<uint8>(); }
        uint16 ReadUInt16() override { return ReadUInt<uint16>(); }
        uint32 ReadUInt32() override { return ReadUInt<uint32>(); }
        float32 ReadFloat() override {
            if (m_json.is_number_float()) {
                return m_json.get<float32>();
            } else {
                return 0.0f;
            }
        }
        String ReadString() override {
            if (m_json.is_string()) {
                return m_json.get<String>();
            } else {
                return String();
            }
        }
        Color ReadColor() {
            if (m_json.is_object()) {
                return Color(m_json["r"].get<float32>(), m_json["g"].get<float32>(), m_json["b"].get<float32>(), m_json["a"].get<float32>());
            } else {
                return Color();
            }
        }
        Vec2 ReadVec2() {
            if (m_json.is_object()) {
                return Vec2(m_json["x"].get<float32>(), m_json["y"].get<float32>());
            } else {
                return Vec2();
            }
        }
        Vec3 ReadVec3() {
            if (m_json.is_object()) {
                return Vec3(m_json["x"].get<float32>(), m_json["y"].get<float32>(), m_json["z"].get<float32>());
            } else {
                return Vec3();
            }
        }
        Vec4 ReadVec4() {
            if (m_json.is_object()) {
                return Vec4(m_json["x"].get<float32>(), m_json["y"].get<float32>(), m_json["z"].get<float32>(), m_json["w"].get<float32>());
            } else {
                return Vec4();
            }
        }
        Quaternion ReadQuaternion() {
            if (m_json.is_object()) {
                return Quaternion(m_json["x"].get<float32>(), m_json["y"].get<float32>(), m_json["z"].get<float32>(), m_json["w"].get<float32>());
            } else {
                return Quaternion();
            }
        }
        void *ReadObject(ReferenceContext &context, SerializableAllocatorFunction allocator) override {
            nlohmann::ordered_json &json_key = m_json;
            if (json_key.is_object()) {
                ISerializable *serializable = allocator();

                JsonDeserializationStream inner_stream;
                inner_stream.m_json = json_key;
                serializable->Deserialize(inner_stream, context);
                return serializable;
            } else if (json_key.is_null()) {
                return nullptr;
            } else {
                return nullptr;
            }
        }
        void ReadStruct(ReferenceContext &context, ISerializable *serializable) override {
            nlohmann::ordered_json &json_key = m_json;
            if (json_key.is_object()) {
                JsonDeserializationStream inner_stream;
                inner_stream.m_json = json_key;
                serializable->Deserialize(inner_stream, context);
            }
        }
        void ReadArray(ReferenceContext &context, ArrayReaderCallback callback) override {
            nlohmann::ordered_json &json_key = m_json;
            if (json_key.is_array()) {
                uint64 index = 0;
                for (nlohmann::ordered_json &element : json_key) {
                    JsonArrayReader reader = JsonArrayReader(element);
                    callback(index, reader);
                }
            }
        }
    private:
        template<typename T>
        T ReadInt() {
            nlohmann::ordered_json json_key = m_json;
            if (json_key.is_number_integer()) {
                return json_key.get<T>();
            } else {
                return 0;
            }
        }

        template<typename T>
        T ReadUInt() {
            nlohmann::ordered_json json_key = m_json;
            if (json_key.is_number_unsigned()) {
                return json_key.get<T>();
            } else {
                return 0;
            }
        }
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

    float32 JsonDeserializationStream::ReadFloat(const char *key) {
        HYP_ASSERT(key);

        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_number_float()) {
            return json_key.get<float32>();
        } else {
            return 0.0f;
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
    Color JsonDeserializationStream::ReadColor(const char *key) {
        HYP_ASSERT(key);
        
        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_object()) {
            return Color(json_key["r"].get<float32>(), json_key["g"].get<float32>(), json_key["b"].get<float32>(), json_key["a"].get<float32>());
        } else {
            return Color();
        }
    }

    //--------------------------------------------------------------
    Vec2 JsonDeserializationStream::ReadVec2(const char *key) {
        HYP_ASSERT(key);
        
        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_object()) {
            return Vec2(json_key["x"].get<float32>(), json_key["y"].get<float32>());
        } else {
            return Vec2();
        }
    }

    //--------------------------------------------------------------
    Vec3 JsonDeserializationStream::ReadVec3(const char *key) {
        HYP_ASSERT(key);

        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_object()) {
            return Vec3(json_key["x"].get<float32>(), json_key["y"].get<float32>(), json_key["z"].get<float32>());
        } else {
            return Vec3();
        }
    }

    //--------------------------------------------------------------
    Vec4 JsonDeserializationStream::ReadVec4(const char *key) {
        HYP_ASSERT(key);
       
        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_object()) {
            return Vec4(json_key["x"].get<float32>(), json_key["y"].get<float32>(), json_key["z"].get<float32>(), json_key["w"].get<float32>());
        } else {
            return Vec4();
        }
    }

    //--------------------------------------------------------------
    Quaternion JsonDeserializationStream::ReadQuaternion(const char *key) {
        HYP_ASSERT(key);

        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_object()) {
            return Quaternion(json_key["x"].get<float32>(), json_key["y"].get<float32>(), json_key["z"].get<float32>(), json_key["w"].get<float32>());
        } else {
            return Quaternion();
        }
    }

    //--------------------------------------------------------------
    void *JsonDeserializationStream::ReadObject(const char *key, ReferenceContext &context, SerializableAllocatorFunction allocator) {
        HYP_ASSERT(key);
        HYP_ASSERT(allocator);

        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_object()) {
            ISerializable *serializable = allocator();

            JsonDeserializationStream inner_stream;
            inner_stream.m_json = json_key;
            serializable->Deserialize(inner_stream, context);
            return serializable;
        } else if (json_key.is_null()) {
            return nullptr;
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    void JsonDeserializationStream::ReadStruct(const char *key, ReferenceContext &context, ISerializable *serializable) {
        HYP_ASSERT(key);
        HYP_ASSERT(serializable);

        nlohmann::ordered_json &json_key = m_json[key];
        if (json_key.is_object()) {
            JsonDeserializationStream inner_stream;
            inner_stream.m_json = json_key;
            serializable->Deserialize(inner_stream, context);
        }
    }

    //--------------------------------------------------------------
    void JsonDeserializationStream::ReadArray(const char *key, ReferenceContext &context, ArrayReaderCallback callback) {
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