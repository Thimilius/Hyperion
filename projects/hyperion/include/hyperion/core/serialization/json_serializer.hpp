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

            return SerializeInternal(object, type);
        }

        template<typename T>
        Result<T, Error> DeserializeCopy(const String &json) {
            Type type = Type::get<T>();
            if (!type.is_valid()) {
                return Error::TypeInvalidForSerialization;
            }
            
            T object = { };
            DeserializeInternal(json, object, type);
            return object;
        }

        template<typename T>
        Result<T *, Error> DeserializeRaw(const String &json) {
            Type type = Type::get<T>();
            if (!type.is_valid()) {
                return Error::TypeInvalidForSerialization;
            }

            Variant object = type.create();
            DeserializeInternal(json, object, type);
            return object.get_value<T *>();
        }
    private:
        String SerializeInternal(Instance object, Type type);
        void DeserializeInternal(const String &json, Instance object, Type type);
    private:
        JsonSerializerSettings m_settings;
    };

}