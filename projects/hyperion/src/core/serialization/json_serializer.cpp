#include "hyppch.hpp"

#include "hyperion/core/serialization/json_serializer.hpp"

#include <nlohmann/json.hpp>
#include <rttr/registration>

#include "hyperion/core/io/file_system.hpp"

namespace Hyperion {

    void JsonSerializer::SerializeInternal(const String &filepath, rttr::instance object, rttr::type type) {
        nlohmann::json json;

        for (auto &property : type.get_properties()) {
            auto variant = property.get_value(object);
            auto property_type = property.get_type();

            if (property_type == rttr::type::get<String>()) {
                json[property.get_name().to_string()] = variant.to_string();
            } else if (property_type == rttr::type::get<int32>()) {
                json[property.get_name().to_string()] = variant.to_int32();
            }
        }

        FileSystem::WriteAllText(filepath, json.dump(m_settings.indent_width));
    }

    void JsonSerializer::DeserializeInternal(const String &filepath, rttr::instance object, rttr::type type) {
        String json_text = FileSystem::ReadAllText(filepath);
        auto json = nlohmann::json::parse(json_text, nullptr, false);

        if (json.is_discarded()) {
            return;
        }
        
        for (auto &property : type.get_properties()) {
            auto &json_property = json[property.get_name().to_string()];
            auto property_type = property.get_type();

            rttr::variant property_variant;
            if (property_type == rttr::type::get<String>()) {
                if (json_property.is_string()) {
                    property_variant = json_property.get<String>();
                }
            } else if (property_type == rttr::type::get<int32>()) {
                if (json_property.is_number_integer()) {
                    property_variant  = json_property.get<int32>();
                }
            }

            property.set_value(object, property_variant);
        }
    }

}