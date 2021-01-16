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
        void Serialize(const String &filepath, const T &object) {
            rttr::type type = rttr::type::get<T>();
            if (!type.is_valid()) {
                return;
            }

            SerializeInternal(filepath, object, type);
        }

        template<typename T>
        T Deserialize(const String &filepath) {
            rttr::type type = rttr::type::get<T>();
            if (!type.is_valid()) {
                return T();
            }

            T object = { };
            DeserializeInternal(filepath, object, type);
            return object;
        }
    private:
        void SerializeInternal(const String &filepath, rttr::instance object, rttr::type type);
        void DeserializeInternal(const String &filepath, rttr::instance object, rttr::type type);
    private:
        JsonSerializerSettings m_settings;
    };

}