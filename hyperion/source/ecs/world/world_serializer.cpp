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
  Array<MetaProperty> GetPropertiesToSerialize(MetaType type) {
    Array<MetaProperty> properties;
    type.ForEachProperty([&properties](MetaProperty property) {
      MetaAttribute property_serialize_attribute = property.GetAttribute(PropertyAttribute::Serialize);
      if (property_serialize_attribute) {
        Any property_attribute_value = property_serialize_attribute.GetValue();
        if (property_attribute_value.GetType().GetPrimitiveType() == MetaPrimitiveType::Bool) {
          bool8 should_serialize = property_attribute_value.Cast<bool8>();
          if (!should_serialize) {
            return;
          }
        }
      }

      properties.Add(property);
    });
    return properties;
  }

  //--------------------------------------------------------------
  void SerializeType(YAML::Emitter &yaml_emitter, World *world, MetaType type, void *instance) {
    Array<MetaProperty> properties = GetPropertiesToSerialize(type);
    if (properties.IsEmpty()) {
      yaml_emitter << YAML::Null;
      return;
    }

    MetaHandle handle = MetaHandle(type, instance);

    yaml_emitter << YAML::BeginMap;
    for (auto it = properties.rbegin(); it != properties.rend(); ++it) {
      MetaProperty property = *it;

      MetaType property_type = property.GetType();
      yaml_emitter << YAML::Key << property.GetName() << YAML::Value;
      Any property_value = property.Get(handle);

      // Check for special property treatment when serializing.
      MetaAttribute property_serialize_as_attribute = property.GetAttribute(PropertyAttribute::SpecialSerialize);
      if (property_serialize_as_attribute) {
        Any property_serialize_as_attribute_value = property_serialize_as_attribute.GetValue();
        PropertySpecialSerialize serialize_as = property_serialize_as_attribute_value.Cast<PropertySpecialSerialize>();
        switch (serialize_as) {
          case PropertySpecialSerialize::Raw: break;
          case PropertySpecialSerialize::EntityIdAsGuid: {
            HYP_ASSERT(property_type == MetaRegistry::Resolve<EntityId>());

            EntityId entity_id = property_value.Cast<EntityId>();
            if (entity_id == Entity::EMPTY) {
              yaml_emitter << YAML::Null;
            } else {
              yaml_emitter << world->GetGuid(entity_id).ToString();
            }

            continue;
          }
          case PropertySpecialSerialize::PointerAsAssetGuid: {
            // NOTE: This way of serializing references to assets feels a little hacky.
            HYP_ASSERT(property_type.IsPointer());

            Asset *asset = *reinterpret_cast<Asset **>(property_value.GetData());
            if (asset) {
              yaml_emitter << asset->GetAssetInfo().guid.ToString();
            } else {
              yaml_emitter << YAML::Null;
            }

            continue;
          }
          default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            break;
        }
      }

      MetaPrimitiveType primitive_type = property_type.GetPrimitiveType();

      switch (primitive_type) {
        case MetaPrimitiveType::Bool: yaml_emitter << property_value.Cast<bool8>();
          break;
        case MetaPrimitiveType::Int8: yaml_emitter << property_value.Cast<int8>();
          break;
        case MetaPrimitiveType::Int16: yaml_emitter << property_value.Cast<int16>();
          break;
        case MetaPrimitiveType::Int32: yaml_emitter << property_value.Cast<int32>();
          break;
        case MetaPrimitiveType::Int64: yaml_emitter << property_value.Cast<int64>();
          break;
        case MetaPrimitiveType::UInt8: yaml_emitter << property_value.Cast<uint8>();
          break;
        case MetaPrimitiveType::UInt16: yaml_emitter << property_value.Cast<uint16>();
          break;
        case MetaPrimitiveType::UInt32: yaml_emitter << property_value.Cast<uint32>();
          break;
        case MetaPrimitiveType::UInt64: yaml_emitter << property_value.Cast<uint64>();
          break;
        case MetaPrimitiveType::Float32: yaml_emitter << property_value.Cast<float32>();
          break;
        case MetaPrimitiveType::Float64: yaml_emitter << property_value.Cast<float64>();
          break;
        case MetaPrimitiveType::Vector2: yaml_emitter << property_value.Cast<Vector2>();
          break;
        case MetaPrimitiveType::Vector3: yaml_emitter << property_value.Cast<Vector3>();
          break;
        case MetaPrimitiveType::Vector4: yaml_emitter << property_value.Cast<Vector4>();
          break;
        case MetaPrimitiveType::Quaternion: yaml_emitter << property_value.Cast<Quaternion>();
          break;
        case MetaPrimitiveType::Matrix4x4: yaml_emitter << property_value.Cast<Matrix4x4>();
          break;
        case MetaPrimitiveType::String: yaml_emitter << property_value.Cast<String>();
          break;
        case MetaPrimitiveType::Color: yaml_emitter << property_value.Cast<Color>();
          break;
        case MetaPrimitiveType::None: {
          if (property_type.IsEnum()) {
            switch (property_type.GetSize()) {
              case 1: yaml_emitter << *reinterpret_cast<int8 *>(property_value.GetData());
                break;
              case 2: yaml_emitter << *reinterpret_cast<int16 *>(property_value.GetData());
                break;
              case 4: yaml_emitter << *reinterpret_cast<int32 *>(property_value.GetData());
                break;
              case 8: yaml_emitter << *reinterpret_cast<int64 *>(property_value.GetData());
                break;
              default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
                break;
            }
          } else if (!property_type.IsPointer() && property_type.IsClass()) {
            SerializeType(yaml_emitter, world, property_type, property_value.GetData());
          } else {
            HYP_LOG_WARN("Serializer", "Unsupported property type {} on property: {}::{}", property_type.GetName(), handle.GetType().GetName(),
                         property.GetName());
          }
          break;
        }
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
          break;
      }
    }
    yaml_emitter << YAML::EndMap;
  }

  void DeserializeType(YAML::Node yaml, World *world, MetaType type, void *instance) {
    Array<MetaProperty> properties = GetPropertiesToSerialize(type);
    if (properties.IsEmpty()) {
      return;
    }

    MetaHandle handle = MetaHandle(type, instance);
    for (MetaProperty property : properties) {
      MetaType property_type = property.GetType();

      String property_name = property.GetName();
      YAML::Node yaml_property = yaml[property_name];

      MetaPrimitiveType primitive_type = property_type.GetPrimitiveType();

      MetaAttribute property_serialize_as_attribute = property.GetAttribute(PropertyAttribute::SpecialSerialize);
      if (property_serialize_as_attribute) {
        Any property_serialize_as_attribute_value = property_serialize_as_attribute.GetValue();
        PropertySpecialSerialize serialize_as = property_serialize_as_attribute_value.Cast<PropertySpecialSerialize>();
        switch (serialize_as) {
          case PropertySpecialSerialize::Raw: break;
          case PropertySpecialSerialize::EntityIdAsGuid: {
            HYP_ASSERT(property_type == MetaRegistry::Resolve<EntityId>());

            if (yaml_property.IsNull()) {
              property.Set(handle, Entity::EMPTY);
            } else {
              EntityGuid guid = EntityGuid::Generate(yaml_property.as<String>());
              EntityId id = world->GetByGuid(guid);
              property.Set(handle, id);
            }

            continue;
          }
          case PropertySpecialSerialize::PointerAsAssetGuid: {
            // NOTE: This way of serializing references to assets feels a little hacky.

            HYP_ASSERT(property_type.IsPointer());

            if (yaml_property.IsNull()) {
              property.Set(handle, nullptr);
            } else {
              AssetGuid guid = AssetGuid::Generate(yaml_property.as<String>());
              if (property_type == MetaRegistry::Resolve<Material *>()) {
                Material *material = AssetManager::GetMaterialByGuid(guid);
                property.Set(handle, material);
              } else if (property_type == MetaRegistry::Resolve<Mesh *>()) {
                Mesh *mesh = AssetManager::GetMeshByGuid(guid);
                property.Set(handle, mesh);
              }
            }

            continue;
          }
          default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            break;
        }
      }

      switch (primitive_type) {
        case MetaPrimitiveType::Bool: property.Set(handle, yaml_property.as<bool8>());
          break;
        case MetaPrimitiveType::Int8: property.Set(handle, yaml_property.as<int8>());
          break;
        case MetaPrimitiveType::Int16: property.Set(handle, yaml_property.as<int16>());
          break;
        case MetaPrimitiveType::Int32: property.Set(handle, yaml_property.as<int32>());
          break;
        case MetaPrimitiveType::Int64: property.Set(handle, yaml_property.as<int64>());
          break;
        case MetaPrimitiveType::UInt8: property.Set(handle, yaml_property.as<uint8>());
          break;
        case MetaPrimitiveType::UInt16: property.Set(handle, yaml_property.as<uint16>());
          break;
        case MetaPrimitiveType::UInt32: property.Set(handle, yaml_property.as<uint32>());
          break;
        case MetaPrimitiveType::UInt64: property.Set(handle, yaml_property.as<uint64>());
          break;
        case MetaPrimitiveType::Float32: property.Set(handle, yaml_property.as<float32>());
          break;
        case MetaPrimitiveType::Float64: property.Set(handle, yaml_property.as<float64>());
          break;
        case MetaPrimitiveType::Vector2: property.Set(handle, yaml_property.as<Vector2>());
          break;
        case MetaPrimitiveType::Vector3: property.Set(handle, yaml_property.as<Vector3>());
          break;
        case MetaPrimitiveType::Vector4: property.Set(handle, yaml_property.as<Vector4>());
          break;
        case MetaPrimitiveType::Quaternion: property.Set(handle, yaml_property.as<Quaternion>());
          break;
        case MetaPrimitiveType::Matrix4x4: property.Set(handle, yaml_property.as<Matrix4x4>());
          break;
        case MetaPrimitiveType::String: property.Set(handle, yaml_property.as<String>());
          break;
        case MetaPrimitiveType::Color: property.Set(handle, yaml_property.as<Color>());
          break;
        case MetaPrimitiveType::None: {
          if (property_type.IsEnum()) {
            switch (property_type.GetSize()) {
              case 1: property.Set(handle, yaml_property.as<int8>());
                break;
              case 2: property.Set(handle, yaml_property.as<int16>());
                break;
              case 4: property.Set(handle, yaml_property.as<int32>());
                break;
              case 8: property.Set(handle, yaml_property.as<int64>());
                break;
              default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
                break;
            }
          } else if (!property_type.IsPointer() && property_type.IsClass()) {
            // NOTE: This might not be the best way to do it as it creates unnecessary copies of the property.
            Any property_value = property.Get(handle);
            DeserializeType(yaml_property, world, property_type, property_value.GetData());
            property.Set(handle, property_value);
          } else {
            HYP_LOG_WARN("Serializer", "Unsupported property type {} on property: {}::{}", property_type.GetName(), handle.GetType().GetName(),
                         property.GetName());
          }
          break;
        }
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
          break;
      }
    }
  }

  //--------------------------------------------------------------
  String WorldSerializer::Serialize(World *world) {
    HYP_PROFILE_SCOPE("WorldSerializer.Serialize");

    YAML::Emitter yaml_emitter;

    yaml_emitter << YAML::BeginMap;
    yaml_emitter << YAML::Key << "name" << YAML::Value << world->GetName();
    yaml_emitter << YAML::Key << "hierarchy" << YAML::Value << YAML::BeginMap;
    {
      WorldHierarchy *world_hierarchy = world->GetHierarchy();
      yaml_emitter << YAML::Key << "root_count" << YAML::Value << world_hierarchy->GetRootCount();
      yaml_emitter << YAML::Key << "first_root" << YAML::Value << world->GetGuid(world_hierarchy->GetFirstRoot()).ToString();
      yaml_emitter << YAML::Key << "last_root" << YAML::Value << world->GetGuid(world_hierarchy->GetLastRoot()).ToString();
    }
    yaml_emitter << YAML::EndMap;

    const Array<ComponentInfo> &component_infos = ComponentRegistry::GetComponentInfos();

    yaml_emitter << YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;
    auto view = world->GetView();
    for (EntityId entity : view) {
      yaml_emitter << YAML::BeginMap;
      yaml_emitter << YAML::Key << "Entity" << YAML::Value << world->GetGuid(entity).ToString();

      for (const ComponentInfo &component_info : component_infos) {
        MetaType component_type = component_info.type;
        void *component = world->GetComponent(component_info.id, entity);
        MetaHandle component_handle = MetaHandle(component_type, component);
        if (component) {
          yaml_emitter << YAML::Key << component_type.GetName();
          SerializeType(yaml_emitter, world, component_type, component);
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
            world->CreateEntity(EntityPrimitive::Empty, guid);
          }
        }
      }
    }

    // In the second phase we do a detailed load of the entities which includes resolving references.

    YAML::Node yaml_hierarchy = yaml_world["hierarchy"];
    if (yaml_hierarchy && yaml_hierarchy.IsMap()) {
      WorldHierarchy *world_hierarchy = world->GetHierarchy();

      YAML::Node yaml_hierarchy_root_count = yaml_hierarchy["root_count"];
      if (yaml_hierarchy_root_count) {
        world_hierarchy->m_root_count = yaml_hierarchy_root_count.as<uint64>();
      }
      YAML::Node yaml_hierarchy_first_root = yaml_hierarchy["first_root"];
      if (yaml_hierarchy_first_root) {
        world_hierarchy->m_first_root = yaml_hierarchy_first_root.IsNull()
                                          ? Entity::EMPTY
                                          : world->GetByGuid(EntityGuid::Generate(yaml_hierarchy_first_root.as<String>()));
      }
      YAML::Node yaml_hierarchy_last_root = yaml_hierarchy["last_root"];
      if (yaml_hierarchy_last_root) {
        world_hierarchy->m_last_root = yaml_hierarchy_last_root.IsNull()
                                         ? Entity::EMPTY
                                         : world->GetByGuid(EntityGuid::Generate(yaml_hierarchy_last_root.as<String>()));
      }
    }

    if (yaml_entities && yaml_entities.IsSequence()) {
      for (auto yaml_entity : yaml_entities) {
        if (yaml_entity && yaml_entity.IsMap()) {
          YAML::Node yaml_entity_guid = yaml_entity["Entity"];
          if (yaml_entity_guid && yaml_entity_guid.IsScalar()) {
            EntityGuid guid = EntityGuid::Generate(yaml_entity_guid.as<String>());
            EntityId entity = world->GetByGuid(guid);

            for (auto pair : yaml_entity) {
              String component_type_string = pair.first.as<String>();

              if (component_type_string == "Entity") {
                continue;
              }

              MetaType component_type = MetaRegistry::Resolve(component_type_string);
              if (!component_type) {
                HYP_LOG_ERROR("Serializer", "Failed to resolve component type: {}", component_type_string);
                continue;
              }

              YAML::Node yaml_component = pair.second;

              ComponentId component_id = ComponentRegistry::GetId(component_type);
              void *component = world->AddComponent(component_id, entity);
              DeserializeType(yaml_component, world, component_type, component);
            }
          }
        }
      }
    }

    return world;
  }

}
