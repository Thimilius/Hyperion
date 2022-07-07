//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world_serializer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/components.hpp"
#include "hyperion/ecs/world/world_manager.hpp"
#include "hyperion/modules/yaml/yaml_serialization.hpp"

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
      } else if (property_type == Type::get<UUID>()) {
        yaml_emitter << property_value.get_value<UUID>().ToString();
      } else if (property_type == Type::get<Color>()) {
        yaml_emitter << property_value.get_value<Color>();
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
      } else if (property_type == Type::get<EntityId>()) {
        EntityId id = property_value.get_value<EntityId>();
        if (id == EntityId::EMPTY) {
          yaml_emitter << YAML::Null;  
        } else {
          yaml_emitter << manager->GetUUID(id).ToString();
        }
      } else if (property_type == Type::get<AssetHandle>()) {
        yaml_emitter << property_value.get_value<AssetHandle>().handle.ToString();
      } else if (property_type.is_pointer()) {
        void *pointer = *static_cast<void **>(Reflection::GetVariantData(property_value));
        
        if (pointer) {
          SerializeType(yaml_emitter, manager, property_type, pointer);
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
      } else if (property_type == Type::get<String>() || property_type == Type::get<UUID>()) {
        setting_property_success = property.set_value(instance, yaml_property.as<String>());
      } else if (property_type == Type::get<Color>()) {
        setting_property_success = property.set_value(instance, yaml_property.as<Color>());
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
      } else if (property_type == Type::get<EntityId>()) {
        EntityId id;
        if (!yaml_property.IsNull()) {
          EntityUUID uuid = EntityUUID::Generate(yaml_property.as<String>());
          id = manager->GetByUUID(uuid);
        }
        setting_property_success = property.set_value(instance, id);
      } else if (property_type == Type::get<AssetHandle>()) {
        AssetHandle handle;
        if (!yaml_property.IsNull()) {
          handle = AssetHandleType::Generate(yaml_property.as<String>());
        }
        setting_property_success = property.set_value(instance, handle);
      } else if (property_type.is_pointer()) {
        Variant property_value = property.get_value(instance);
        
        if (yaml_property.IsNull()) {
          property_value = nullptr;            
        } else {
          void *pointer = *static_cast<void **>(Reflection::GetVariantData(property_value));
          DeserializeType(yaml_property, manager, property_type, pointer);
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
    HYP_PROFILE_SCOPE("WorldSerializer.Serialize")

    EntityManager *manager = world->GetEntityManager();
    
    YAML::Emitter yaml_emitter;

    yaml_emitter << YAML::BeginMap;
    yaml_emitter << YAML::Key << "name" << YAML::Value << world->GetName();
    yaml_emitter << YAML::Key << "hierarchy" << YAML::Value << YAML::BeginMap;
    {
      EntityHierarchy *hierarchy = world->GetHierarchy();
      yaml_emitter << YAML::Key << "root_count" << YAML::Value << hierarchy->GetRootCount();
      yaml_emitter << YAML::Key << "first_root" << YAML::Value << manager->GetUUID(hierarchy->GetFirstRoot()).ToString();
      yaml_emitter << YAML::Key << "last_root" << YAML::Value << manager->GetUUID(hierarchy->GetLastRoot()).ToString();
    }
    yaml_emitter << YAML::EndMap;

    const Array<ComponentInfo> &component_infos = ComponentRegistry::GetComponentInfos();

    yaml_emitter << YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;
    auto view = manager->GetViewAll();
    for (EntityId entity : view) {
      yaml_emitter << YAML::BeginMap;
      yaml_emitter << YAML::Key << "Entity" << YAML::Value << manager->GetUUID(entity).ToString();

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
    HYP_PROFILE_SCOPE("WorldSerializer.Deserialize")

    // TEMP: For now we assume that assets referenced by components are already loaded.
    // Later we should verify that every AssetHandle we have at the end actually points to a loaded asset.
    
    YAML::Node yaml_world = YAML::Load(data);
    World *world = WorldManager::CreateWorld();
    EntityManager *manager = world->GetEntityManager();

    YAML::Node yaml_name = yaml_world["name"];
    if (yaml_name) {
      world->SetName(yaml_name.as<String>());
    }

    // In the first phase we just create all entities with their corresponding UUID.
    YAML::Node yaml_entities = yaml_world["entities"];
    if (yaml_entities && yaml_entities.IsSequence()) {
      for (auto yaml_entity : yaml_entities) {
        if (yaml_entity && yaml_entity.IsMap()) {
          YAML::Node yaml_entity_uuid = yaml_entity["Entity"];
          if (yaml_entity_uuid && yaml_entity_uuid.IsScalar()) {
            EntityUUID uuid = EntityUUID::Generate(yaml_entity_uuid.as<String>());
            manager->CreateEntity(EntityPrimitive::Empty, uuid);
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
                                        : manager->GetByUUID(EntityUUID::Generate(yaml_hierarchy_first_root.as<String>()));
      }
      YAML::Node yaml_hierarchy_last_root = yaml_hierarchy["last_root"];
      if (yaml_hierarchy_last_root) {
        world_hierarchy->m_last_root = yaml_hierarchy_last_root.IsNull()
                                       ? EntityId::EMPTY
                                       : manager->GetByUUID(EntityUUID::Generate(yaml_hierarchy_last_root.as<String>()));
      }
    }

    if (yaml_entities && yaml_entities.IsSequence()) {
      for (auto yaml_entity : yaml_entities) {
        if (yaml_entity && yaml_entity.IsMap()) {
          YAML::Node yaml_entity_uuid = yaml_entity["Entity"];
          if (yaml_entity_uuid && yaml_entity_uuid.IsScalar()) {
            EntityUUID uuid = EntityUUID::Generate(yaml_entity_uuid.as<String>());
            EntityId entity = manager->GetByUUID(uuid);

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
