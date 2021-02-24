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

    //--------------------------------------------------------------
    void SerializeObject(nlohmann::ordered_json &json_object, Instance object, Type type) {
        for (auto &property : type.get_properties()) {
            // First make sure the property actually needs to be serialized based on the metadata.
            Variant property_metadata = property.get_metadata(Metadata::DontSerialize);
            if (property_metadata.is_valid() && property_metadata.get_value<bool>()) {
                continue;
            }

            Variant property_variant = property.get_value(object);
            Type property_type = property.get_type();
            String property_name = property.get_name().to_string();

            if (property_type == Type::get<String>()) {
                json_object[property_name] = property_variant.to_string();
            } else if (property_type == Type::get<bool>()) {
                json_object[property_name] = property_variant.to_bool();
            } else if (property_type == Type::get<int32>()) {
                json_object[property_name] = property_variant.to_int32();
            } else if (property_type == Type::get<float32>()) {
                json_object[property_name] = property_variant.to_float();
            } else if (property_type == Type::get<Guid>()) {
                Guid guid = property_variant.get_value<Guid>();
                json_object[property_name] = guid.ToString();
            } else if (property_type == Type::get<Vec2>()) {
                Vec2 vec2 = property_variant.get_value<Vec2>();
                json_object[property_name] = nlohmann::json::object({ { "x", vec2.x }, { "y", vec2.y } });
            } else if (property_type == Type::get<Vec3>()) {
                Vec3 vec3 = property_variant.get_value<Vec3>();
                json_object[property_name] = nlohmann::json::object({ { "x", vec3.x }, { "y", vec3.y }, { "z", vec3.z } });
            } else if (property_type == Type::get<Vec4>()) {
                Vec4 vec4 = property_variant.get_value<Vec4>();
                json_object[property_name] = nlohmann::json::object({ { "x", vec4.x }, { "y", vec4.y }, { "z", vec4.z }, { "w", vec4.w } });
            } else if (property_type == Type::get<Color>()) {
                Color color = property_variant.get_value<Color>();
                json_object[property_name] = nlohmann::json::object({ { "r", color.r }, { "g", color.g }, { "b", color.b }, { "a", color.a } });
            } else if (property_type.is_enumeration()) {
                auto enumeration = property_type.get_enumeration();
                Variant enum_metadata = enumeration.get_metadata(Metadata::Flags);
                if (enum_metadata.is_valid() && enum_metadata.get_value<bool>()) {
                    Type underlying_type = enumeration.get_underlying_type();
                    if (underlying_type == Type::get<int32>()) {
                        json_object[property_name] = property_variant.get_value<int32>();
                    } else if (underlying_type == Type::get<uint32>()) {
                        json_object[property_name] = property_variant.get_value<uint32>();
                    } else {
                        HYP_ASSERT(false);
                    }
                } else {
                    HYP_ASSERT(false);
                }
            } else if (property_type.is_pointer()) {
                json_object[property_name] = nullptr;
            } else {
                Type sub_object_type = property_variant.get_type();
                if (sub_object_type.is_pointer()) {
                    json_object[property_name] = nullptr;
                } else {
                    nlohmann::ordered_json sub_object = nlohmann::json::object();
                    SerializeObject(sub_object, property_variant, sub_object_type);
                    json_object[property_name] = sub_object;
                }
            }
        }
    }

    //--------------------------------------------------------------
    String JsonSerializer::SerializeInternal(Instance object, Type type) {
        nlohmann::ordered_json json_object = nlohmann::ordered_json::object();
        SerializeObject(json_object, object, type);
        return json_object.dump(m_settings.indent_width);
    }

    //--------------------------------------------------------------
    void DeserializeObject(nlohmann::ordered_json &json_object, Instance object, Type type) {
        for (auto &property : type.get_properties()) {
            // First make sure the property actually needs to be deserialized based on the metadata.
            Variant metadata = property.get_metadata(Metadata::DontSerialize);
            if (metadata.is_valid() && metadata.get_value<bool>()) {
                continue;
            }

            String property_name = property.get_name().to_string();
            auto &json_property_it = json_object.find(property_name);
            if (json_property_it == json_object.end()) {
                HYP_LOG_ERROR("Engine", "Failed to deserialize property '{}' of type '{}'!", property_name, type.get_name().to_string());
                continue;
            }

            auto &json_property = *json_property_it;
            Type property_type = property.get_type();

            Variant property_variant;
            if (property_type == Type::get<String>() && json_property.is_string()) {
                property_variant = json_property.get<String>();
            } else if (property_type == Type::get<bool>() && json_property.is_boolean()) {
                property_variant = json_property.get<bool>();
            } else if (property_type == Type::get<int32>() && json_property.is_number_integer()) {
                property_variant = json_property.get<int32>();
            } else if (property_type == Type::get<float32>() && json_property.is_number_float()) {
                property_variant = json_property.get<float32>();
            } else if (property_type == Type::get<Guid>() && json_property.is_string()) {
                property_variant = Guid::Create(json_property.get<String>());
            } else if (property_type == Type::get<Vec2>() && json_property.is_object()) {
                property_variant = Vec2(json_property["x"].get<float32>(), json_property["y"].get<float32>());
            } else if (property_type == Type::get<Vec3>() && json_property.is_object()) {
                property_variant = Vec3(json_property["x"].get<float32>(), json_property["y"].get<float32>(), json_property["z"].get<float32>());
            } else if (property_type == Type::get<Vec4>() && json_property.is_object()) {
                property_variant = Vec4(json_property["x"].get<float32>(), json_property["y"].get<float32>(), json_property["z"].get<float32>(), json_property["w"].get<float32>());
            } else if (property_type == Type::get<Color>() && json_property.is_object()) {
                property_variant = Color(json_property["r"].get<float32>(), json_property["g"].get<float32>(), json_property["b"].get<float32>(), json_property["a"].get<float32>());
            } else if (property_type.is_enumeration()) {
                auto enumeration = property_type.get_enumeration();
                Variant enum_metadata = enumeration.get_metadata(Metadata::Flags);
                if (enum_metadata.is_valid() && enum_metadata.get_value<bool>()) {
                    Type underlying_type = enumeration.get_underlying_type();
                    if (underlying_type == Type::get<int32>()) {
                        property_variant = json_property.get<int32>();
                    } else if (underlying_type == Type::get<uint32>()) {
                        property_variant = json_property.get<uint32>();
                    } else {
                        HYP_ASSERT(false);
                    }

                    if (!property_variant.convert(static_cast<Type>(property_type))) {
                        HYP_LOG_ERROR("Engine", "Failed to set convert enum property '{}' when deserializing type '{}'!", property_name, type.get_name().to_string());
                    }
                } else {
                    HYP_ASSERT(false);
                }
            } else if (property_type.is_pointer()) {

            } else {
                property_variant = property.get_value(object);
                Type sub_object_type = property_variant.get_type();
                DeserializeObject(json_property, property_variant, sub_object_type);
            }

            if (!property.set_value(object, property_variant)) {
                HYP_LOG_ERROR("Engine", "Failed to set property '{}' when deserializing type '{}'!", property_name, type.get_name().to_string());
            }
        }
    }

    //--------------------------------------------------------------
    void JsonSerializer::DeserializeInternal(const String &json, Instance object, Type type) {
        auto json_object = nlohmann::ordered_json::parse(json, nullptr, false);

        if (json_object.is_discarded()) {
            return;
        }
        
        DeserializeObject(json_object, object, type);
    }

}