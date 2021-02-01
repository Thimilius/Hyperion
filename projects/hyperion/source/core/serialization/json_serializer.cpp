#include "hyppch.hpp"

#include "hyperion/core/serialization/json_serializer.hpp"

#include <nlohmann/json.hpp>
#include <rttr/registration>

#include "hyperion/core/io/file_system.hpp"

namespace Hyperion {

    String JsonSerializer::SerializeInternal(Instance object, Type type) {
        nlohmann::json json_object;

        for (auto &property : type.get_properties()) {
            auto variant = property.get_value(object);
            auto property_type = property.get_type();
            if (property_type == Type::get<String>()) {
                json_object[property.get_name().to_string()] = variant.to_string();
            } else if (property_type == Type::get<int32>()) {
                json_object[property.get_name().to_string()] = variant.to_int32();
            }
        }

        return json_object.dump(m_settings.indent_width);
    }

    void JsonSerializer::DeserializeInternal(const String &json, Instance object, Type type) {
        auto json_object = nlohmann::json::parse(json, nullptr, false);

        if (json_object.is_discarded()) {
            return;
        }
        
        for (auto &property : type.get_properties()) {
            auto &json_property = json_object[property.get_name().to_string()];
            auto property_type = property.get_type();

            Variant property_variant;
            if (property_type == Type::get<String>()) {
                if (json_property.is_string()) {
                    property_variant = json_property.get<String>();
                }
            } else if (property_type == Type::get<int32>()) {
                if (json_property.is_number_integer()) {
                    property_variant  = json_property.get<int32>();
                }
            }

            property.set_value(object, property_variant);
        }
    }

}