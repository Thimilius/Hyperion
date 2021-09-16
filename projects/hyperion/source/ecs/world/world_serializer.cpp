//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world_serializer.hpp"

//---------------------- Library Includes ----------------------
#include <yaml-cpp/yaml.h>

//---------------------- Project Includes ----------------------
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

        static bool decode(const Node &node, Hyperion::Vector2 &vector) {
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

        static bool decode(const Node &node, Hyperion::Vector3 &vector) {
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

        static bool decode(const Node &node, Hyperion::Vector4 &vector) {
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

        static bool decode(const Node &node, Hyperion::Quaternion &quaternion) {
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

        static bool decode(const Node &node, Hyperion::Color &color) {
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

}


//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    String WorldSerializer::Serialize(World *world) {
        YAML::Emitter yaml_emitter;

        yaml_emitter << YAML::BeginMap;
        yaml_emitter << YAML::Key << "name" << YAML::Value << world->GetName();
        yaml_emitter << YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;

        auto view = world->GetView();
        for (EntityId entity : view) {
            yaml_emitter << YAML::BeginMap;
            yaml_emitter << YAML::Key << "Entity" << YAML::Value << world->GetGuid(entity).ToString();

            TagComponent *tag = world->GetComponent<TagComponent>(entity);
            if (tag != nullptr) {
                yaml_emitter << YAML::Key << "Tag";
                yaml_emitter << YAML::BeginMap;
                {
                    yaml_emitter << YAML::Key << "tag" << YAML::Value << tag->tag;
                }
                yaml_emitter << YAML::EndMap;
            }

            LocalTransformComponent *transform = world->GetComponent<LocalTransformComponent>(entity);
            if (transform != nullptr) {
                yaml_emitter << YAML::Key << "LocalTransform";
                yaml_emitter << YAML::BeginMap;
                {
                    yaml_emitter << YAML::Key << "position" << YAML::Value << transform->position;
                    yaml_emitter << YAML::Key << "rotation" << YAML::Value << transform->rotation;
                    yaml_emitter << YAML::Key << "scale" << YAML::Value << transform->scale;
                }
                yaml_emitter << YAML::EndMap;
            }

            DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(entity);
            if (derived_transform != nullptr) {
                yaml_emitter << YAML::Key << "DerivedTransform" << YAML::Null;
            }

            LocalToWorldComponent *local_to_world = world->GetComponent<LocalToWorldComponent>(entity);
            if (local_to_world != nullptr) {
                yaml_emitter << YAML::Key << "LocalToWorld" << YAML::Null;
            }

            HierarchyComponent *hierarchy = world->GetComponent<HierarchyComponent>(entity);
            if (hierarchy != nullptr) {
                yaml_emitter << YAML::Key << "Hierarchy";
                yaml_emitter << YAML::BeginMap;
                {
                    if (hierarchy->parent == Entity::EMPTY) {
                        yaml_emitter << YAML::Key << "parent" << YAML::Null;
                    } else {
                        yaml_emitter << YAML::Key << "parent" << world->GetGuid(hierarchy->parent).ToString();
                    }

                    if (hierarchy->previous_sibling == Entity::EMPTY) {
                        yaml_emitter << YAML::Key << "previous_sibling" << YAML::Null;
                    } else {
                        yaml_emitter << YAML::Key << "previous_sibling" << world->GetGuid(hierarchy->previous_sibling).ToString();
                    }
                    if (hierarchy->next_sibling == Entity::EMPTY) {
                        yaml_emitter << YAML::Key << "next_sibling" << YAML::Null;
                    } else {
                        yaml_emitter << YAML::Key << "next_sibling" << world->GetGuid(hierarchy->next_sibling).ToString();
                    }

                    yaml_emitter << YAML::Key << "child_count" << hierarchy->child_count;
                    if (hierarchy->first_child == Entity::EMPTY) {
                        yaml_emitter << YAML::Key << "first_child" << YAML::Null;
                    } else {
                        yaml_emitter << YAML::Key << "first_child" << world->GetGuid(hierarchy->first_child).ToString();
                    }
                    if (hierarchy->last_child == Entity::EMPTY) {
                        yaml_emitter << YAML::Key << "last_child" << YAML::Null;
                    } else {
                        yaml_emitter << YAML::Key << "last_child" << world->GetGuid(hierarchy->last_child).ToString();
                    }
                }
                yaml_emitter << YAML::EndMap;
            }

            CameraComponent *camera = world->GetComponent<CameraComponent>(entity);
            if (camera != nullptr) {
                yaml_emitter << YAML::Key << "Camera";
                yaml_emitter << YAML::BeginMap;
                {
                    yaml_emitter << YAML::Key << "projection_mode" << static_cast<uint64>(camera->projection_mode);

                    yaml_emitter << YAML::Key << "clear_mode" << static_cast<uint64>(camera->clear_mode);
                    yaml_emitter << YAML::Key << "background_color" << camera->background_color;

                    yaml_emitter << YAML::Key << "near_plane" << camera->near_plane;
                    yaml_emitter << YAML::Key << "far_plane" << camera->far_plane;

                    yaml_emitter << YAML::Key << "fov" << camera->fov;
                    yaml_emitter << YAML::Key << "orthographic_size" << camera->orthographic_size;

                    yaml_emitter << YAML::Key << "viewport_clipping";
                    yaml_emitter << YAML::Flow;
                    yaml_emitter << YAML::BeginMap;
                    yaml_emitter << YAML::Key << "x" << YAML::Value << camera->viewport_clipping.x;
                    yaml_emitter << YAML::Key << "y" << YAML::Value << camera->viewport_clipping.y;
                    yaml_emitter << YAML::Key << "width" << YAML::Value << camera->viewport_clipping.width;
                    yaml_emitter << YAML::Key << "height" << YAML::Value << camera->viewport_clipping.height;
                    yaml_emitter << YAML::EndMap;
                }
                yaml_emitter << YAML::EndMap;
            }

            SpriteComponent *sprite = world->GetComponent<SpriteComponent>(entity);
            if (sprite != nullptr) {
                yaml_emitter << YAML::Key << "Sprite" << YAML::Null;
            }

            RenderMeshComponent *render_mesh = world->GetComponent<RenderMeshComponent>(entity);
            if (render_mesh != nullptr) {
                yaml_emitter << YAML::Key << "RenderMesh";
                yaml_emitter << YAML::BeginMap;
                {
                    yaml_emitter << YAML::Key << "material" << YAML::Value << render_mesh->material->GetAssetInfo().guid.ToString();
                    yaml_emitter << YAML::Key << "mesh" << YAML::Value << render_mesh->mesh->GetAssetInfo().guid.ToString();
                    yaml_emitter << YAML::Key << "sub_mesh_index" << YAML::Value << render_mesh->sub_mesh_index;
                }
                yaml_emitter << YAML::EndMap;
            }

            yaml_emitter << YAML::EndMap;
        }

        yaml_emitter << YAML::EndSeq;
        yaml_emitter << YAML::EndMap;

        return String(yaml_emitter.c_str());
    }

    //--------------------------------------------------------------
    World *WorldSerializer::Deserialize(const String &data) {
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
        if (yaml_entities && yaml_entities.IsSequence()) {
            for (auto yaml_entity : yaml_entities) {
                if (yaml_entity && yaml_entity.IsMap()) {
                    YAML::Node yaml_entity_guid = yaml_entity["Entity"];
                    if (yaml_entity_guid && yaml_entity_guid.IsScalar()) {
                        EntityGuid guid = EntityGuid::Generate(yaml_entity_guid.as<String>());
                        EntityId entity = world->GetByGuid(guid);

                        YAML::Node yaml_tag = yaml_entity["Tag"];
                        if (yaml_tag && yaml_tag.IsMap()) {
                            TagComponent *tag = world->AddComponent<TagComponent>(entity);
                            
                            YAML::Node yaml_tag_tag = yaml_tag["tag"];
                            if (yaml_tag_tag && yaml_tag_tag.IsScalar()) {
                                tag->tag = yaml_tag_tag.as<String>();
                            }
                        }

                        YAML::Node yaml_local_transform = yaml_entity["LocalTransform"];
                        if (yaml_local_transform && yaml_local_transform.IsMap()) {
                            LocalTransformComponent *local_transform = world->AddComponent<LocalTransformComponent>(entity);

                            YAML::Node yaml_local_transform_position = yaml_local_transform["position"];
                            if (yaml_local_transform_position) {
                                local_transform->position = yaml_local_transform_position.as<Vector3>();
                            }
                            YAML::Node yaml_local_transform_rotation = yaml_local_transform["rotation"];
                            if (yaml_local_transform_rotation) {
                                local_transform->rotation = yaml_local_transform_rotation.as<Quaternion>();
                            }
                            YAML::Node yaml_local_transform_scale = yaml_local_transform["scale"];
                            if (yaml_local_transform_scale) {
                                local_transform->scale = yaml_local_transform_scale.as<Vector3>();
                            }
                        }
                        YAML::Node yaml_derived_transform = yaml_entity["DerivedTransform"];
                        if (yaml_derived_transform && yaml_derived_transform.IsMap()) {
                            world->AddComponent<DerivedTransformComponent>(entity);
                        }
                        YAML::Node yaml_local_to_world = yaml_entity["LocalToWorld"];
                        if (yaml_local_to_world && yaml_local_to_world.IsMap()) {
                            world->AddComponent<LocalToWorldComponent>(entity);
                        }
                        YAML::Node yaml_hierarchy = yaml_entity["Hierarchy"];
                        if (yaml_hierarchy && yaml_hierarchy.IsMap()) {
                            HierarchyComponent *hierarchy = world->AddComponent<HierarchyComponent>(entity);

                            YAML::Node yaml_hierarchy_parent = yaml_hierarchy["parent"];
                            if (yaml_hierarchy_parent) {
                                hierarchy->parent = yaml_hierarchy_parent.IsNull() ? Entity::EMPTY : world->GetByGuid(EntityGuid::Generate(yaml_hierarchy_parent.as<String>()));
                            }
                            YAML::Node yaml_hierarchy_previous_sibling = yaml_hierarchy["previous_sibling"];
                            if (yaml_hierarchy_previous_sibling) {
                                hierarchy->previous_sibling = yaml_hierarchy_previous_sibling.IsNull() ? Entity::EMPTY : world->GetByGuid(EntityGuid::Generate(yaml_hierarchy_previous_sibling.as<String>()));
                            }
                            YAML::Node yaml_hierarchy_next_sibling = yaml_hierarchy["next_sibling"];
                            if (yaml_hierarchy_next_sibling) {
                                hierarchy->next_sibling = yaml_hierarchy_next_sibling.IsNull() ? Entity::EMPTY : world->GetByGuid(EntityGuid::Generate(yaml_hierarchy_next_sibling.as<String>()));
                            }
                            YAML::Node yaml_hierarchy_child_count = yaml_hierarchy["child_count"];
                            if (yaml_hierarchy_child_count) {
                                hierarchy->child_count = yaml_hierarchy_child_count.as<uint64>();
                            }
                            YAML::Node yaml_hierarchy_first_child = yaml_hierarchy["first_child"];
                            if (yaml_hierarchy_first_child) {
                                hierarchy->first_child = yaml_hierarchy_first_child.IsNull() ? Entity::EMPTY : world->GetByGuid(EntityGuid::Generate(yaml_hierarchy_first_child.as<String>()));
                            }
                            YAML::Node yaml_hierarchy_last_child = yaml_hierarchy["last_child"];
                            if (yaml_hierarchy_last_child) {
                                hierarchy->last_child = yaml_hierarchy_last_child.IsNull() ? Entity::EMPTY : world->GetByGuid(EntityGuid::Generate(yaml_hierarchy_last_child.as<String>()));
                            }
                        }

                        YAML::Node yaml_camera = yaml_entity["Camera"];
                        if (yaml_camera && yaml_camera.IsMap()) {
                            CameraComponent *camera = world->AddComponent<CameraComponent>(entity);

                            YAML::Node yaml_camera_projection_mode = yaml_camera["projection_mode"];
                            if (yaml_camera_projection_mode) {
                                camera->projection_mode = static_cast<Rendering::CameraProjectionMode>(yaml_camera_projection_mode.as<uint64>());
                            }
                            YAML::Node yaml_camera_clear_mode = yaml_camera["projection_mode"];
                            if (yaml_camera_clear_mode) {
                                camera->clear_mode = static_cast<Rendering::CameraClearMode>(yaml_camera_clear_mode.as<uint64>());
                            }
                            YAML::Node yaml_camera_background_color = yaml_camera["background_color"];
                            if (yaml_camera_background_color) {
                                camera->background_color = yaml_camera_background_color.as<Color>();
                            }
                            YAML::Node yaml_camera_near_plane = yaml_camera["near_plane"];
                            if (yaml_camera_near_plane) {
                                camera->near_plane = yaml_camera_near_plane.as<float32>();
                            }
                            YAML::Node yaml_camera_far_plane = yaml_camera["far_plane"];
                            if (yaml_camera_far_plane) {
                                camera->far_plane = yaml_camera_far_plane.as<float32>();
                            }
                            YAML::Node yaml_camera_fov = yaml_camera["fov"];
                            if (yaml_camera_fov) {
                                camera->fov = yaml_camera_fov.as<float32>();
                            }
                            YAML::Node yaml_camera_orthographic_size = yaml_camera["orthographic_size"];
                            if (yaml_camera_orthographic_size) {
                                camera->orthographic_size = yaml_camera_orthographic_size.as<float32>();
                            }
                            YAML::Node yaml_camera_viewport_clipping = yaml_camera["viewport_clipping"];
                            if (yaml_camera_viewport_clipping && yaml_camera_viewport_clipping.IsMap()) {
                                Rendering::CameraViewportClipping viewport_clipping;

                                YAML::Node yaml_camera_viewport_clipping_x = yaml_camera_viewport_clipping["x"];
                                if (yaml_camera_viewport_clipping_x) {
                                    viewport_clipping.x = yaml_camera_viewport_clipping_x.as<float32>();
                                }
                                YAML::Node yaml_camera_viewport_clipping_y = yaml_camera_viewport_clipping["y"];
                                if (yaml_camera_viewport_clipping_y) {
                                    viewport_clipping.y = yaml_camera_viewport_clipping_y.as<float32>();
                                }
                                YAML::Node yaml_camera_viewport_clipping_width = yaml_camera_viewport_clipping["width"];
                                if (yaml_camera_viewport_clipping_width) {
                                    viewport_clipping.width = yaml_camera_viewport_clipping_width.as<float32>();
                                }
                                YAML::Node yaml_camera_viewport_clipping_height = yaml_camera_viewport_clipping["height"];
                                if (yaml_camera_viewport_clipping_height) {
                                    viewport_clipping.height = yaml_camera_viewport_clipping_height.as<float32>();
                                }

                                camera->viewport_clipping = viewport_clipping;
                            }
                        }
                        YAML::Node yaml_sprite = yaml_entity["Sprite"];
                        if (yaml_sprite && yaml_sprite.IsMap()) {
                            SpriteComponent *sprite = world->AddComponent<SpriteComponent>(entity);
                        }
                        YAML::Node yaml_render_mesh = yaml_entity["RenderMesh"];
                        if (yaml_render_mesh && yaml_render_mesh.IsMap()) {
                            RenderMeshComponent *render_mesh = world->AddComponent<RenderMeshComponent>(entity);
                        }
                    }
                }
            }
        }

        return world;
    }

}
