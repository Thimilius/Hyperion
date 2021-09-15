//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world_serializer.hpp"

//---------------------- Library Includes ----------------------
#include <yaml-cpp/yaml.h>

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/components.hpp"

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
                    yaml_emitter << YAML::Key << "position";
                    yaml_emitter << YAML::Flow;
                    yaml_emitter << YAML::BeginMap;
                    yaml_emitter << YAML::Key << "x" << YAML::Value << transform->position.x;
                    yaml_emitter << YAML::Key << "y" << YAML::Value << transform->position.y;
                    yaml_emitter << YAML::Key << "z" << YAML::Value << transform->position.z;
                    yaml_emitter << YAML::EndMap;

                    yaml_emitter << YAML::Key << "rotation";
                    yaml_emitter << YAML::Flow;
                    yaml_emitter << YAML::BeginMap;
                    yaml_emitter << YAML::Key << "x" << YAML::Value << transform->rotation.x;
                    yaml_emitter << YAML::Key << "y" << YAML::Value << transform->rotation.y;
                    yaml_emitter << YAML::Key << "z" << YAML::Value << transform->rotation.z;
                    yaml_emitter << YAML::Key << "w" << YAML::Value << transform->rotation.w;
                    yaml_emitter << YAML::EndMap;

                    yaml_emitter << YAML::Key << "scale";
                    yaml_emitter << YAML::Flow;
                    yaml_emitter << YAML::BeginMap;
                    yaml_emitter << YAML::Key << "x" << YAML::Value << transform->scale.x;
                    yaml_emitter << YAML::Key << "y" << YAML::Value << transform->scale.y;
                    yaml_emitter << YAML::Key << "z" << YAML::Value << transform->scale.z;
                    yaml_emitter << YAML::EndMap;
                }
                yaml_emitter << YAML::EndMap;
            }

            HierarchyComponent *hierarchy = world->GetComponent<HierarchyComponent>(entity);
            if (hierarchy != nullptr) {
                yaml_emitter << YAML::Key << "Hierarchy" << YAML::Flow << YAML::BeginMap << YAML::EndMap;
            }

            LocalToWorldComponent *local_to_world = world->GetComponent<LocalToWorldComponent>(entity);
            if (local_to_world != nullptr) {
                yaml_emitter << YAML::Key << "LocalToWorld" << YAML::Flow << YAML::BeginMap << YAML::EndMap;
            }

            CameraComponent *camera = world->GetComponent<CameraComponent>(entity);
            if (camera != nullptr) {
                yaml_emitter << YAML::Key << "Camera";
                yaml_emitter << YAML::BeginMap;
                {
                    yaml_emitter << YAML::Key << "projection_mode" << static_cast<uint64>(camera->projection_mode);

                    yaml_emitter << YAML::Key << "clear_mode" << static_cast<uint64>(camera->clear_mode);
                    yaml_emitter << YAML::Key << "background_color";
                    yaml_emitter << YAML::Flow;
                    yaml_emitter << YAML::BeginMap;
                    yaml_emitter << YAML::Key << "r" << YAML::Value << camera->background_color.r;
                    yaml_emitter << YAML::Key << "g" << YAML::Value << camera->background_color.g;
                    yaml_emitter << YAML::Key << "b" << YAML::Value << camera->background_color.b;
                    yaml_emitter << YAML::Key << "a" << YAML::Value << camera->background_color.a;
                    yaml_emitter << YAML::EndMap;

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
                yaml_emitter << YAML::Key << "Sprite" << YAML::Flow << YAML::BeginMap << YAML::EndMap;
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
        return nullptr;
    }

}