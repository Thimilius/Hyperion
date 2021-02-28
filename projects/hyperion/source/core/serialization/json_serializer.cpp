//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/serialization/json_serializer.hpp"

//---------------------- Library Includes ----------------------
#include <nlohmann/json.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/io/file_system.hpp"
#include "hyperion/core/object/guid.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    void SerializeVariant(nlohmann::ordered_json &json, const Variant &variant, Type type) {
        if (type == Type::get<bool>()) {
            json = variant.to_bool();
        } else if (type == Type::get<int8>() || type == Type::get<int16>() || type == Type::get<int32>()) {
            json = variant.to_int32();
        } else if (type == Type::get<uint8>() || type == Type::get<uint16>() || type == Type::get<uint32>()) {
            json = variant.to_uint32();
        } else if (type == Type::get<int64>() || type == Type::get<uint64>()) {
            // NOTE: JSON does not really support 64 bit integer. So for now we do not really handle them at all.
            HYP_ASSERT(false);
        } else if (type == Type::get<String>()) {
            json = variant.to_string();
        } else if (type == Type::get<float32>() || type == Type::get<float64>()) {
            json = variant.to_float();
        } else if (type == Type::get<Guid>()) {
            Guid guid = variant.get_value<Guid>();
            json = guid.ToString();
        } else if (type == Type::get<Vec2>()) {
            Vec2 vec2 = variant.get_value<Vec2>();
            json = nlohmann::ordered_json::object({ { "x", vec2.x }, { "y", vec2.y } });
        } else if (type == Type::get<Vec3>()) {
            Vec3 vec3 = variant.get_value<Vec3>();
            json = nlohmann::ordered_json::object({ { "x", vec3.x }, { "y", vec3.y }, { "z", vec3.z } });
        } else if (type == Type::get<Vec4>()) {
            Vec4 vec4 = variant.get_value<Vec4>();
            json = nlohmann::ordered_json::object({ { "x", vec4.x }, { "y", vec4.y }, { "z", vec4.z }, { "w", vec4.w } });
        } else if (type == Type::get<Quaternion>()) {
            Quaternion quaternion = variant.get_value<Quaternion>();
            json = nlohmann::ordered_json::object({ { "x", quaternion.x }, { "y", quaternion.y }, { "z", quaternion.z }, { "w", quaternion.w } });
        } else if (type == Type::get<Color>()) {
            Color color = variant.get_value<Color>();
            json = nlohmann::ordered_json::object({ { "r", color.r }, { "g", color.g }, { "b", color.b }, { "a", color.a } });
        } else if (type.is_enumeration()) {
            auto enumeration = type.get_enumeration();
            Variant enum_metadata = enumeration.get_metadata(Metadata::Flags);
            if (enum_metadata.is_valid() && enum_metadata.get_value<bool>()) {
                // Enum flags get saved as their underlying integer type.
                Type underlying_type = enumeration.get_underlying_type();
                SerializeVariant(json, variant, underlying_type);
            } else {
                // Normal enums get saved as strings representing the corresponding value.
                // TODO: Implement.
                HYP_ASSERT(false);
            }
        } else if (type.is_sequential_container()) {
            auto sequential_view = variant.create_sequential_view();
            json = nlohmann::ordered_json::array();
            for (const Variant &wrapped_element_variant : sequential_view) {
                Variant element_variant = wrapped_element_variant.extract_wrapped_value();
                Type element_type = element_variant.get_type();

                nlohmann::ordered_json json_element;
                SerializeVariant(json_element, element_variant, element_type);
                json.push_back(json_element);
            }
        } else {
            // If its not a primitive type, we have three possibilities left:
            //     - An embedded object
            //     - A pointer to a object
            //     - A pointer to a primitive type

            if (type.is_pointer()) {
                // NOTE: Pointers will have to be handled in a special way, to keep track of all references.

                // A pointer that is null is an early bail out.
                if (variant == nullptr) {
                    json = nullptr;
                    return;
                }
            }

            json = nlohmann::ordered_json::object();
            for (auto &property : type.get_properties()) {
                // First make sure the property actually needs to be serialized based on the metadata.
                Variant property_metadata = property.get_metadata(Metadata::DontSerialize);
                if (property_metadata.is_valid() && property_metadata.get_value<bool>()) {
                    continue;
                }

                String property_name = property.get_name().to_string();
                Type property_type = property.get_type();
                Variant property_variant = property.get_value(variant);
                nlohmann::ordered_json property_json_object;
                SerializeVariant(property_json_object, property_variant, property_type);
                json[property_name] = property_json_object;
            }
        }
    }

    //--------------------------------------------------------------
    String JsonSerializer::SerializeInternal(Variant &variant, Type type) {
        nlohmann::ordered_json json;
        SerializeVariant(json, variant, type);
        return json.dump(m_settings.indent_width);
    }

    //--------------------------------------------------------------
    void DeserializeVariant(nlohmann::ordered_json &json, Variant &variant, Type type) {
        if (type == Type::get<bool>()) {
            variant = json.get<bool>();
        } else if (type == Type::get<int8>() || type == Type::get<int16>() || type == Type::get<int32>()) {
            variant = json.get<int32>();
        } else if (type == Type::get<uint8>() || type == Type::get<uint16>() || type == Type::get<uint32>()) {
            variant = json.get<uint32>();
        } else if (type == Type::get<int64>() || type == Type::get<uint64>()) {
            // NOTE: JSON does not really support 64 bit integer. So for now we do not really handle them at all.
            HYP_ASSERT(false);
        } else if (type == Type::get<String>()) {
            variant = json.get<String>();
        } else if (type == Type::get<float32>() || type == Type::get<float64>()) {
            variant = json.get<float32>();
        } else if (type == Type::get<Guid>()) {
            variant = Guid::Create(json.get<String>());
        } else if (type == Type::get<Vec2>()) {
            variant = Vec2(json["x"].get<float32>(), json["y"].get<float32>());
        } else if (type == Type::get<Vec3>()) {
            variant = Vec3(json["x"].get<float32>(), json["y"].get<float32>(), json["z"].get<float32>());
        } else if (type == Type::get<Vec4>()) {
            variant = Vec4(json["x"].get<float32>(), json["y"].get<float32>(), json["z"].get<float32>(), json["w"].get<float32>());
        } else if (type == Type::get<Quaternion>()) {
            variant = Quaternion(json["x"].get<float32>(), json["y"].get<float32>(), json["z"].get<float32>(), json["w"].get<float32>());
        } else if (type == Type::get<Color>()) {
            variant = Color(json["r"].get<float32>(), json["g"].get<float32>(), json["b"].get<float32>(), json["a"].get<float32>());
        } else if (type.is_enumeration()) {
            auto enumeration = type.get_enumeration();
            Variant enum_metadata = enumeration.get_metadata(Metadata::Flags);
            if (enum_metadata.is_valid() && enum_metadata.get_value<bool>()) {
                // Enum flags get saved as their underlying integer type.
                Type underlying_type = enumeration.get_underlying_type();
                SerializeVariant(json, variant, underlying_type);

                if (!variant.convert(static_cast<Type>(type))) {
                    HYP_LOG_ERROR("Engine", "Failed to convert enum when deserializing type '{}'!", type.get_name().to_string());
                }
            } else {
                // Normal enums get saved as strings representing the corresponding value.
                // TODO: Implement.
                HYP_ASSERT(false);
            }
        } else if (type.is_sequential_container()) {
            auto sequential_view = variant.create_sequential_view();
            uint64 size = json.size();
            sequential_view.set_size(size);

            for (uint64 i = 0; i < size; i++) {
                nlohmann::ordered_json json_element = json[i];
                Variant element_variant = sequential_view.get_value(i).extract_wrapped_value();
                Type element_type = element_variant.get_type();
                DeserializeVariant(json_element, element_variant, element_type);
                if (!sequential_view.set_value(i, element_variant)) {
                    HYP_LOG_ERROR("Engine", "Failed to set array element of type '{}' when deserializing!", element_type.get_name().to_string());
                }
            }
        } else {
            if (type.is_pointer()) {
                // NOTE: Pointers will have to be handled in a special way, to keep track of all references.

                // A pointer that is null is an early bail out.
                if (json.is_null()) {
                    variant = nullptr;
                    return;
                }
            }

            for (auto &property : type.get_properties()) {
                // First make sure the property actually needs to be serialized based on the metadata.
                Variant property_metadata = property.get_metadata(Metadata::DontSerialize);
                if (property_metadata.is_valid() && property_metadata.get_value<bool>()) {
                    continue;
                }

                String property_name = property.get_name().to_string();
                Type property_type = property.get_type();
                Variant property_variant = property.get_value(variant);
                nlohmann::ordered_json property_json_object = json[property_name];
                DeserializeVariant(property_json_object, property_variant, property_type);
                if (!property.set_value(variant, property_variant)) {
                    HYP_LOG_ERROR("Engine", "Failed to set object property of type '{}' when deserializing!", property_type.get_name().to_string());
                }
            }
        }
    }

    //--------------------------------------------------------------
    void JsonSerializer::DeserializeInternal(const String &json_text, Variant &variant, Type type) {
        nlohmann::ordered_json json_object = nlohmann::ordered_json::parse(json_text, nullptr, false);

        if (json_object.is_discarded()) {
            return;
        }

        DeserializeVariant(json_object, variant, type);
    }

}