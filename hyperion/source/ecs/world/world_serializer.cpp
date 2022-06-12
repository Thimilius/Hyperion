//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world_serializer.hpp"

//---------------------- Library Includes ----------------------
#include <yaml-cpp/yaml.h>

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/ecs/component/components/components.hpp"
#include "hyperion/ecs/world/world_manager.hpp"

namespace YAML {

  //--------------------------------------------------------------
  template<>
  struct convert<Hyperion::Vector2> {
    static Node encode(const Hyperion::Vector2 &vector) {
      Node node;
      node.SetStyle(EmitterStyle::Flow);
      node["x"] = vector.x;
      node["y"] = vector.y;
      return node;
    }

    static bool8 decode(const Node &node, Hyperion::Vector2 &vector) {
      if (!node.IsMap() || node.size() != 2) {
        return false;
      }

      auto yaml_x = node["x"];
      if (!yaml_x || !yaml_x.IsScalar()) {
        return false;
      }
      vector.x = yaml_x.as<float32>();

      auto yaml_y = node["y"];
      if (!yaml_y || !yaml_y.IsScalar()) {
        return false;
      }
      vector.y = yaml_y.as<float32>();

      return true;
    }
  };

  //--------------------------------------------------------------
  Emitter &operator <<(Emitter &emitter, const Hyperion::Vector2 &vector) {
    emitter << Flow;
    emitter << BeginMap;
    emitter << Key << "x" << Value << vector.x;
    emitter << Key << "y" << Value << vector.y;
    emitter << EndMap;
    return emitter;
  }

  //--------------------------------------------------------------
  template<>
  struct convert<Hyperion::Vector3> {
    static Node encode(const Hyperion::Vector3 &vector) {
      Node node;
      node.SetStyle(EmitterStyle::Flow);
      node["x"] = vector.x;
      node["y"] = vector.y;
      node["z"] = vector.z;
      return node;
    }

    static bool8 decode(const Node &node, Hyperion::Vector3 &vector) {
      if (!node.IsMap() || node.size() != 3) {
        return false;
      }

      auto yaml_x = node["x"];
      if (!yaml_x || !yaml_x.IsScalar()) {
        return false;
      }
      vector.x = yaml_x.as<float32>();

      auto yaml_y = node["y"];
      if (!yaml_y || !yaml_y.IsScalar()) {
        return false;
      }
      vector.y = yaml_y.as<float32>();

      auto yaml_z = node["z"];
      if (!yaml_z || !yaml_z.IsScalar()) {
        return false;
      }
      vector.z = yaml_z.as<float32>();

      return true;
    }
  };

  //--------------------------------------------------------------
  Emitter &operator <<(Emitter &emitter, const Hyperion::Vector3 &vector) {
    emitter << Flow;
    emitter << BeginMap;
    emitter << Key << "x" << Value << vector.x;
    emitter << Key << "y" << Value << vector.y;
    emitter << Key << "z" << Value << vector.z;
    emitter << EndMap;
    return emitter;
  }

  //--------------------------------------------------------------
  template<>
  struct convert<Hyperion::Vector4> {
    static Node encode(const Hyperion::Vector4 &vector) {
      Node node;
      node.SetStyle(EmitterStyle::Flow);
      node["x"] = vector.x;
      node["y"] = vector.y;
      node["z"] = vector.z;
      node["w"] = vector.w;
      return node;
    }

    static bool8 decode(const Node &node, Hyperion::Vector4 &vector) {
      if (!node.IsMap() || node.size() != 4) {
        return false;
      }

      auto yaml_x = node["x"];
      if (!yaml_x || !yaml_x.IsScalar()) {
        return false;
      }
      vector.x = yaml_x.as<float32>();

      auto yaml_y = node["y"];
      if (!yaml_y || !yaml_y.IsScalar()) {
        return false;
      }
      vector.y = yaml_y.as<float32>();

      auto yaml_z = node["z"];
      if (!yaml_z || !yaml_z.IsScalar()) {
        return false;
      }
      vector.z = yaml_z.as<float32>();

      auto yaml_w = node["w"];
      if (!yaml_w || !yaml_w.IsScalar()) {
        return false;
      }
      vector.w = yaml_w.as<float32>();

      return true;
    }
  };

  //--------------------------------------------------------------
  Emitter &operator <<(Emitter &emitter, const Hyperion::Vector4 &vector) {
    emitter << Flow;
    emitter << BeginMap;
    emitter << Key << "x" << Value << vector.x;
    emitter << Key << "y" << Value << vector.y;
    emitter << Key << "z" << Value << vector.z;
    emitter << Key << "w" << Value << vector.w;
    emitter << EndMap;
    return emitter;
  }

  //--------------------------------------------------------------
  template<>
  struct convert<Hyperion::Quaternion> {
    static Node encode(const Hyperion::Quaternion &quaternion) {
      Node node;
      node.SetStyle(EmitterStyle::Flow);
      node["x"] = quaternion.x;
      node["y"] = quaternion.y;
      node["z"] = quaternion.z;
      node["w"] = quaternion.w;
      return node;
    }

    static bool8 decode(const Node &node, Hyperion::Quaternion &quaternion) {
      if (!node.IsMap() || node.size() != 4) {
        return false;
      }

      auto yaml_x = node["x"];
      if (!yaml_x || !yaml_x.IsScalar()) {
        return false;
      }
      quaternion.x = yaml_x.as<float32>();

      auto yaml_y = node["y"];
      if (!yaml_y || !yaml_y.IsScalar()) {
        return false;
      }
      quaternion.y = yaml_y.as<float32>();

      auto yaml_z = node["z"];
      if (!yaml_z || !yaml_z.IsScalar()) {
        return false;
      }
      quaternion.z = yaml_z.as<float32>();

      auto yaml_w = node["w"];
      if (!yaml_w || !yaml_w.IsScalar()) {
        return false;
      }
      quaternion.w = yaml_w.as<float32>();

      return true;
    }
  };

  //--------------------------------------------------------------
  Emitter &operator <<(Emitter &emitter, const Hyperion::Quaternion &quaternion) {
    emitter << Flow;
    emitter << BeginMap;
    emitter << Key << "x" << Value << quaternion.x;
    emitter << Key << "y" << Value << quaternion.y;
    emitter << Key << "z" << Value << quaternion.z;
    emitter << Key << "w" << Value << quaternion.w;
    emitter << EndMap;
    return emitter;
  }

  //--------------------------------------------------------------
  template<>
  struct convert<Hyperion::Color> {
    static Node encode(const Hyperion::Color &color) {
      Node node;
      node.SetStyle(EmitterStyle::Flow);
      node["r"] = color.r;
      node["g"] = color.g;
      node["b"] = color.b;
      node["a"] = color.a;
      return node;
    }

    static bool8 decode(const Node &node, Hyperion::Color &color) {
      if (!node.IsMap() || node.size() != 4) {
        return false;
      }

      auto yaml_r = node["r"];
      if (!yaml_r || !yaml_r.IsScalar()) {
        return false;
      }
      color.r = yaml_r.as<float32>();

      auto yaml_g = node["g"];
      if (!yaml_g || !yaml_g.IsScalar()) {
        return false;
      }
      color.g = yaml_g.as<float32>();

      auto yaml_b = node["b"];
      if (!yaml_b || !yaml_b.IsScalar()) {
        return false;
      }
      color.b = yaml_b.as<float32>();

      auto yaml_a = node["a"];
      if (!yaml_a || !yaml_a.IsScalar()) {
        return false;
      }
      color.a = yaml_a.as<float32>();

      return true;
    }
  };

  //--------------------------------------------------------------
  Emitter &operator <<(Emitter &emitter, const Hyperion::Color &color) {
    emitter << Flow;
    emitter << BeginMap;
    emitter << Key << "r" << Value << color.r;
    emitter << Key << "g" << Value << color.g;
    emitter << Key << "b" << Value << color.b;
    emitter << Key << "a" << Value << color.a;
    emitter << EndMap;
    return emitter;
  }

  //--------------------------------------------------------------
  template<>
  struct convert<Hyperion::Matrix4x4> {
    static Node encode(const Hyperion::Matrix4x4 &matrix) {
      Node node;
      node["row1"] = matrix.GetRow(0);
      node["row2"] = matrix.GetRow(1);
      node["row3"] = matrix.GetRow(2);
      node["row4"] = matrix.GetRow(3);
      return node;
    }

    static bool8 decode(const Node &node, Hyperion::Matrix4x4 &matrix) {
      if (!node.IsMap() || node.size() != 4) {
        return false;
      }

      auto yaml_r = node["row1"];
      if (!yaml_r || !yaml_r.IsMap()) {
        return false;
      }
      matrix.SetRow(0, yaml_r.as<Hyperion::Vector4>());

      auto yaml_g = node["row2"];
      if (!yaml_g || !yaml_g.IsMap()) {
        return false;
      }
      matrix.SetRow(1, yaml_r.as<Hyperion::Vector4>());

      auto yaml_b = node["row3"];
      if (!yaml_b || !yaml_b.IsMap()) {
        return false;
      }
      matrix.SetRow(2, yaml_r.as<Hyperion::Vector4>());

      auto yaml_a = node["row4"];
      if (!yaml_a || !yaml_a.IsMap()) {
        return false;
      }
      matrix.SetRow(3, yaml_r.as<Hyperion::Vector4>());

      return true;
    }
  };

  //--------------------------------------------------------------
  Emitter &operator <<(Emitter &emitter, const Hyperion::Matrix4x4 &matrix) {
    emitter << BeginMap;
    emitter << Key << "row1" << Value << matrix.GetRow(0);
    emitter << Key << "row2" << Value << matrix.GetRow(1);
    emitter << Key << "row3" << Value << matrix.GetRow(2);
    emitter << Key << "row4" << Value << matrix.GetRow(3);
    emitter << EndMap;
    return emitter;
  }

}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  Array<Property> GetPropertiesToSerialize(Type type) {
    Array<Property> properties;
    for (Property property : type.get_properties()) {
      Variant property_metadata_serialize = property.get_metadata(PropertyMetadata::Serialize);
      if (property_metadata_serialize.is_valid()) {
        if (property_metadata_serialize.is_type<bool8>()) {
          bool8 should_serialize = property_metadata_serialize.to_bool();
          if (!should_serialize) {
            continue;
          }
        }
      }
      
      properties.Add(property);
    }
    return properties;
  }

  //--------------------------------------------------------------
  void SerializeType(YAML::Emitter &yaml_emitter, EntityManager *manager, Type type, void *object) {
    Array<Property> properties = GetPropertiesToSerialize(type);
    if (properties.IsEmpty()) {
      yaml_emitter << YAML::Null;
      return;
    }

    Instance instance = Reflection::CreateInstanceFromRaw(type, object);

    yaml_emitter << YAML::BeginMap;
    for (Property property : properties) {
      Type property_type = property.get_type();
      String property_name = property.get_name().to_string();
      Variant property_value = property.get_value(instance);
      
      yaml_emitter << YAML::Key << property_name << YAML::Value;

      // TODO: Add handling of arrays, sequential containers and associative containers
      
      if (property_type.is_arithmetic()) {
        if (property_type == Type::get<bool8>()) {
          yaml_emitter << property_value.to_bool();
        } else if (property_type == Type::get<int8>()) {
          yaml_emitter << property_value.to_int8();
        } else if (property_type == Type::get<int16>()) {
          yaml_emitter << property_value.to_int16();
        } else if (property_type == Type::get<int32>()) {
          yaml_emitter << property_value.to_int32();
        } else if (property_type == Type::get<int64>()) {
          yaml_emitter << property_value.to_int64();
        } else if (property_type == Type::get<uint8>()) {
          yaml_emitter << property_value.to_uint8();
        } else if (property_type == Type::get<uint16>()) {
          yaml_emitter << property_value.to_uint16();
        } else if (property_type == Type::get<uint32>()) {
          yaml_emitter << property_value.to_uint32();
        } else if (property_type == Type::get<uint64>()) {
          yaml_emitter << property_value.to_uint64();
        } else if (property_type == Type::get<float32>()) {
          yaml_emitter << property_value.to_float();
        } else if (property_type == Type::get<float64>()) {
          yaml_emitter << property_value.to_double();
        }
      } else if (property_type.is_enumeration()) {
        uint64 enum_size = property_type.get_sizeof();
        switch (enum_size) {
          case 1: yaml_emitter << property_value.to_uint8(); break;
          case 2: yaml_emitter << property_value.to_uint16(); break;
          case 4: yaml_emitter << property_value.to_uint32(); break;
          case 8: yaml_emitter << property_value.to_uint64(); break;
          default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
      } else if (property_type == Type::get<String>()) {
        yaml_emitter << property_value.to_string();
      } else if (property_type == Type::get<Guid>()) {
        yaml_emitter << property_value.get_value<Guid>().ToString();
      } else if (property_type == Type::get<Vector2>()) {
        yaml_emitter << property_value.get_value<Vector2>();
      } else if (property_type == Type::get<Vector3>()) {
        yaml_emitter << property_value.get_value<Vector3>();
      } else if (property_type == Type::get<Vector4>()) {
        yaml_emitter << property_value.get_value<Vector4>();
      } else if (property_type == Type::get<Quaternion>()) {
        yaml_emitter << property_value.get_value<Quaternion>();
      } else if (property_type == Type::get<Matrix4x4>()) {
        yaml_emitter << property_value.get_value<Matrix4x4>();
      } else if (property_type == Type::get<Color>()) {
        yaml_emitter << property_value.get_value<Color>();
      } else if (property_type == Type::get<EntityId>()) {
        EntityId id = property_value.get_value<EntityId>();
        if (id == EntityId::EMPTY) {
          yaml_emitter << YAML::Null;  
        } else {
          yaml_emitter << manager->GetGuid(id).ToString();
        }
      } else if (property_type.is_pointer()) {
        void *pointer = *static_cast<void **>(Reflection::GetVariantData(property_value));
        
        if (pointer) {
          // We handle assets directly here to save them by their guid.
          Type property_type_raw = property_type.get_raw_type();
          if (property_type_raw == Type::get<Asset>() || property_type_raw.is_derived_from(Type::get<Asset>())) {
            Asset *asset = static_cast<Asset *>(pointer);
            yaml_emitter << asset->GetAssetInfo().handle.handle.ToString();
          } else {
            SerializeType(yaml_emitter, manager, property_type, pointer);  
          }
        } else {
          yaml_emitter << YAML::Null;
        }
      } else if (property_type.is_class()) {
        void *property_object = Reflection::GetVariantData(property_value);
        SerializeType(yaml_emitter, manager, property_type, property_object);
      } else {
        HYP_LOG_WARN(
          "Serializer",
          "Unsupported property type {} on property: {}::{}",
          property_type.get_name().to_string(),
          instance.get_type().get_name().to_string(),
          property.get_name().to_string()
        );
      }
    }
    
    yaml_emitter << YAML::EndMap;
  }

  void DeserializeType(YAML::Node yaml, EntityManager *manager, Type type, void *object) {
    Array<Property> properties = GetPropertiesToSerialize(type);
    if (properties.IsEmpty()) {
      return;
    }

    Instance instance = Reflection::CreateInstanceFromRaw(type, object);
    for (Property property : properties) {
      Type property_type = property.get_type();
      String property_name = property.get_name().to_string();
      
      YAML::Node yaml_property = yaml[property_name];

      bool8 setting_property_success = false; 
      if (property_type.is_arithmetic()) {
        if (property_type == Type::get<bool8>()) {
          setting_property_success = property.set_value(instance, yaml_property.as<bool8>());
        } else if (property_type == Type::get<int8>()) {
          setting_property_success = property.set_value(instance, yaml_property.as<int8>());
        } else if (property_type == Type::get<int16>()) {
          setting_property_success = property.set_value(instance, yaml_property.as<int16>());
        } else if (property_type == Type::get<int32>()) {
          setting_property_success = property.set_value(instance, yaml_property.as<int32>());
        } else if (property_type == Type::get<int64>()) {
          setting_property_success = property.set_value(instance, yaml_property.as<int64>());
        } else if (property_type == Type::get<uint8>()) {
          setting_property_success = property.set_value(instance, yaml_property.as<uint8>());
        } else if (property_type == Type::get<uint16>()) {
          setting_property_success = property.set_value(instance, yaml_property.as<uint16>());
        } else if (property_type == Type::get<uint32>()) {
          setting_property_success = property.set_value(instance, yaml_property.as<uint32>());
        } else if (property_type == Type::get<uint64>()) {
          setting_property_success = property.set_value(instance, yaml_property.as<uint64>());
        } else if (property_type == Type::get<float32>()) {
          setting_property_success = property.set_value(instance, yaml_property.as<float32>());
        } else if (property_type == Type::get<float64>()) {
          setting_property_success = property.set_value(instance, yaml_property.as<float64>());
        }
      } else if (property_type.is_enumeration()) {
        uint64 enum_size = property_type.get_sizeof();
        Variant property_enum_variant;
        switch (enum_size) {
          case 1: property_enum_variant = yaml_property.as<uint8>(); break;
          case 2: property_enum_variant = yaml_property.as<uint16>(); break;
          case 4: property_enum_variant = yaml_property.as<uint32>(); break;
          case 8: property_enum_variant = yaml_property.as<uint64>(); break;
          default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
        const Type &property_type_reference = property_type;
        property_enum_variant.convert(property_type_reference);
        setting_property_success = property.set_value(instance, property_enum_variant);
      } else if (property_type == Type::get<String>() || property_type == Type::get<Guid>()) {
        setting_property_success = property.set_value(instance, yaml_property.as<String>());
      } else if (property_type == Type::get<Vector2>()) {
        setting_property_success = property.set_value(instance, yaml_property.as<Vector2>());
      } else if (property_type == Type::get<Vector3>()) {
        setting_property_success = property.set_value(instance, yaml_property.as<Vector3>());
      } else if (property_type == Type::get<Vector4>()) {
        setting_property_success = property.set_value(instance, yaml_property.as<Vector4>());
      } else if (property_type == Type::get<Quaternion>()) {
        setting_property_success = property.set_value(instance, yaml_property.as<Quaternion>());
      } else if (property_type == Type::get<Matrix4x4>()) {
        setting_property_success = property.set_value(instance, yaml_property.as<Matrix4x4>());
      } else if (property_type == Type::get<Color>()) {
        setting_property_success = property.set_value(instance, yaml_property.as<Color>());
      } else if (property_type == Type::get<EntityId>()) {
        EntityId id;
        if (!yaml_property.IsNull()) {
          EntityGuid guid = EntityGuid::Generate(yaml_property.as<String>());
          id = manager->GetByGuid(guid);
        }
        setting_property_success = property.set_value(instance, id);
      } else if (property_type.is_pointer()) {
        Variant property_value = property.get_value(instance);
        
        if (yaml_property.IsNull()) {
          property_value = nullptr;            
        } else {
          // We handle assets directly here to load them by their guid.
          Type property_type_raw = property_type.get_raw_type();
          if (property_type_raw == Type::get<Asset>() || property_type_raw.is_derived_from(Type::get<Asset>())) {
            AssetHandle guid = AssetHandleType::Generate(yaml_property.as<String>());
            if (property_type_raw == Type::get<Material>()) {
              property_value = AssetManager::GetMaterial(guid);
            } else if (property_type_raw == Type::get<Mesh>()) {
              property_value = AssetManager::GetMesh(guid);
            }
          } else {
            void *pointer = *static_cast<void **>(Reflection::GetVariantData(property_value));
            DeserializeType(yaml_property, manager, property_type, pointer);
          }
        }
        
        setting_property_success = property.set_value(instance, property_value);
      } else if (property_type.is_class()) {
        if (!yaml_property.IsNull()) {
          Variant property_value = property.get_value(instance);
          void *pointer = Reflection::GetVariantData(property_value);
          DeserializeType(yaml_property, manager, property_type, pointer);
          setting_property_success = property.set_value(instance, property_value);  
        }
      } else {
        HYP_LOG_WARN(
          "Serializer",
          "Unsupported property type {} on property: {}::{}",
          property_type.get_name().to_string(),
          instance.get_type().get_name().to_string(),
          property.get_name().to_string()
        );
      }

      if (!setting_property_success) {
        HYP_LOG_ERROR(
          "Serializer",
          "Setting of property {}::{} of type {} was not successful",
          instance.get_type().get_name().to_string(),
          property.get_name().to_string(),
          property_type.get_name().to_string()
        );
      }
    }
  }

  //--------------------------------------------------------------
  String WorldSerializer::Serialize(World *world) {
    HYP_PROFILE_SCOPE("WorldSerializer.Serialize");

    EntityManager *manager = world->GetEntityManager();
    
    YAML::Emitter yaml_emitter;

    yaml_emitter << YAML::BeginMap;
    yaml_emitter << YAML::Key << "name" << YAML::Value << world->GetName();
    yaml_emitter << YAML::Key << "hierarchy" << YAML::Value << YAML::BeginMap;
    {
      EntityHierarchy *hierarchy = world->GetHierarchy();
      yaml_emitter << YAML::Key << "root_count" << YAML::Value << hierarchy->GetRootCount();
      yaml_emitter << YAML::Key << "first_root" << YAML::Value << manager->GetGuid(hierarchy->GetFirstRoot()).ToString();
      yaml_emitter << YAML::Key << "last_root" << YAML::Value << manager->GetGuid(hierarchy->GetLastRoot()).ToString();
    }
    yaml_emitter << YAML::EndMap;

    const Array<ComponentInfo> &component_infos = ComponentRegistry::GetComponentInfos();

    yaml_emitter << YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;
    auto view = manager->GetViewAll();
    for (EntityId entity : view) {
      yaml_emitter << YAML::BeginMap;
      yaml_emitter << YAML::Key << "Entity" << YAML::Value << manager->GetGuid(entity).ToString();

      for (const ComponentInfo &component_info : component_infos) {
        Type component_type = *component_info.type;
        void *component = manager->GetComponent(component_info.id, entity);
        if (component) {
          yaml_emitter << YAML::Key << component_type.get_name().to_string();
          SerializeType(yaml_emitter, manager, component_type, component);
        }
      }

      yaml_emitter << YAML::EndMap;
    }

    yaml_emitter << YAML::EndSeq;
    yaml_emitter << YAML::EndMap;

    return String(yaml_emitter.c_str());
  }

  //--------------------------------------------------------------
  World *WorldSerializer::Deserialize(const String &data) {
    HYP_PROFILE_SCOPE("WorldSerializer.Deserialize");

    // TEMP: For now we assume that assets referenced by components are already loaded and we can just grab the pointer.
    
    YAML::Node yaml_world = YAML::Load(data);
    World *world = WorldManager::CreateWorld();
    EntityManager *manager = world->GetEntityManager();

    YAML::Node yaml_name = yaml_world["name"];
    if (yaml_name) {
      world->SetName(yaml_name.as<String>());
    }

    // In the first phase we just create all entities with their corresponding guid.
    YAML::Node yaml_entities = yaml_world["entities"];
    if (yaml_entities && yaml_entities.IsSequence()) {
      for (auto yaml_entity : yaml_entities) {
        if (yaml_entity && yaml_entity.IsMap()) {
          YAML::Node yaml_entity_guid = yaml_entity["Entity"];
          if (yaml_entity_guid && yaml_entity_guid.IsScalar()) {
            EntityGuid guid = EntityGuid::Generate(yaml_entity_guid.as<String>());
            manager->CreateEntity(EntityPrimitive::Empty, guid);
          }
        }
      }
    }

    // In the second phase we do a detailed load of the entities which includes resolving references.

    YAML::Node yaml_hierarchy = yaml_world["hierarchy"];
    if (yaml_hierarchy && yaml_hierarchy.IsMap()) {
      EntityHierarchy *world_hierarchy = world->GetHierarchy();

      YAML::Node yaml_hierarchy_root_count = yaml_hierarchy["root_count"];
      if (yaml_hierarchy_root_count) {
        world_hierarchy->m_root_count = yaml_hierarchy_root_count.as<uint64>();
      }
      YAML::Node yaml_hierarchy_first_root = yaml_hierarchy["first_root"];
      if (yaml_hierarchy_first_root) {
        world_hierarchy->m_first_root = yaml_hierarchy_first_root.IsNull()
                                        ? EntityId::EMPTY
                                        : manager->GetByGuid(EntityGuid::Generate(yaml_hierarchy_first_root.as<String>()));
      }
      YAML::Node yaml_hierarchy_last_root = yaml_hierarchy["last_root"];
      if (yaml_hierarchy_last_root) {
        world_hierarchy->m_last_root = yaml_hierarchy_last_root.IsNull()
                                       ? EntityId::EMPTY
                                       : manager->GetByGuid(EntityGuid::Generate(yaml_hierarchy_last_root.as<String>()));
      }
    }

    if (yaml_entities && yaml_entities.IsSequence()) {
      for (auto yaml_entity : yaml_entities) {
        if (yaml_entity && yaml_entity.IsMap()) {
          YAML::Node yaml_entity_guid = yaml_entity["Entity"];
          if (yaml_entity_guid && yaml_entity_guid.IsScalar()) {
            EntityGuid guid = EntityGuid::Generate(yaml_entity_guid.as<String>());
            EntityId entity = manager->GetByGuid(guid);

            for (auto pair : yaml_entity) {
              String component_type_string = pair.first.as<String>();

              if (component_type_string == "Entity") {
                continue;
              }

              Type component_type = Type::get_by_name(component_type_string);
              if (!component_type.is_valid()) {
                HYP_LOG_ERROR("Serializer", "Failed to resolve component type: {}", component_type_string);
                continue;
              }

              YAML::Node yaml_component = pair.second;

              ComponentId component_id = ComponentRegistry::GetId(component_type);
              void *component = manager->AddComponent(component_id, entity);
              DeserializeType(yaml_component, manager, component_type, component);
            }
          }
        }
      }
    }

    return world;
  }

}
