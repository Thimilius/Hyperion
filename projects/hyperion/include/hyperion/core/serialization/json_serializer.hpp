#pragma once

#include "hyperion/common.hpp"

#include <rttr/registration>

namespace Hyperion {

    struct JsonSerializerSettings {
        uint32 indent_width = 4;
    };

    class JsonSerializer {
    public:
        JsonSerializer(JsonSerializerSettings settings = JsonSerializerSettings()) : m_settings(settings) { }

        template<typename T>
        String Serialize(const T &object) {
            Type type = Type::get<T>();
            if (!type.is_valid()) {
                return String();
            }

            return SerializeInternal(object, type);
        }

        template<typename T>
        T DeserializeCopy(const String &json) {
            Type type = Type::get<T>();
            if (!type.is_valid()) {
                return T();
            }
            
            T object = { };
            DeserializeInternal(json, object, type);
            return object;
        }

        template<typename T>
        T *DeserializeRaw(const String &json) {
            Type type = Type::get<T>();
            if (!type.is_valid()) {
                return nullptr;
            }

            Variant object = type.create();
            DeserializeInternal(json, object, type);
            return object.get_value<T*>();
        }
    private:
        String SerializeInternal(Instance object, Type type);
        void DeserializeInternal(const String &json, Instance object, Type type);
    private:
        JsonSerializerSettings m_settings;
    };

}