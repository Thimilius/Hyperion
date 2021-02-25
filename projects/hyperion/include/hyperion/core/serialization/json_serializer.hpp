#pragma once

//---------------------- Library Includes ----------------------
#include <rttr/registration>

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct JsonSerializerSettings {
        uint32 indent_width = 4;
    };

    class JsonSerializer {
    public:
        JsonSerializer(JsonSerializerSettings settings = JsonSerializerSettings()) : m_settings(settings) { }
    public:
        template<typename T>
        Result<String, Error> Serialize(const T &object) {
            Type type = Type::get<T>();
            if (!type.is_valid()) {
                return Error::TypeInvalidForSerialization;
            }

            Variant variant = object;
            return SerializeInternal(variant, type);
        }

        template<typename T>
        Result<T, Error> DeserializeCopy(const String &json) {
            Type type = Type::get<T>();
            if (!type.is_valid()) {
                return Error::TypeInvalidForSerialization;
            }
            
            Variant variant = T{ };
            DeserializeInternal(json, variant, type);
            return variant.get_value<T>();
        }

        template<typename T>
        Result<T *, Error> DeserializeRaw(const String &json) {
            Type type = Type::get<T>();
            if (!type.is_valid()) {
                return Error::TypeInvalidForSerialization;
            }

            Variant variant = type.create();
            DeserializeInternal(json, variant, type);
            return variant.get_value<T *>();
        }
    private:
        String SerializeInternal(Variant &variant, Type type);
        void DeserializeInternal(const String &json_text, Variant &variant, Type type);
    private:
        JsonSerializerSettings m_settings;
    };

}