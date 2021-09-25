//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/components.hpp"
#include "hyperion/ecs/system/systems/asset_systems.hpp"
#include "hyperion/ecs/system/systems/physics_systems.hpp"
#include "hyperion/ecs/system/systems/transform_systems.hpp"
#include "hyperion/ecs/system/systems/render_systems.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    World *WorldManager::CreateWorld() {
        HYP_PROFILE_SCOPE("WorldManager.CreateWorld");

        World *world = new World();
        s_worlds.Add(world);
        return world;
    }

    //--------------------------------------------------------------
    void WorldManager::DestroyWorld(World *world) {
        HYP_PROFILE_SCOPE("WorldManager.DestroyWorld");

        HYP_ASSERT(world);

        if (s_active_world == world) {
            HYP_LOG_WARN("Entity", "Trying to destroy currently active world.");
            return;
        } else {
            s_worlds.Remove(world);
            delete world;
        }
    }

    //--------------------------------------------------------------
    void WorldManager::SetActiveWorld(World *world) {
        s_active_world = world;
    }

    //--------------------------------------------------------------
    void WorldManager::ReflectTypes() {
        // Components
        {
            MetaRegistry::Reflect<IComponent>("IComponent");

            MetaRegistry::Reflect<NameComponent>("NameComponent")
                .Base<IComponent>()
                .Property<&NameComponent::name>("name");
            MetaRegistry::Reflect<TagComponent>("TagComponent")
                .Base<IComponent>()
                .Property<&TagComponent::tag>("tag");
            MetaRegistry::Reflect<DisabledComponent>("DisabledComponent")
                .Base<IComponent>();
            MetaRegistry::Reflect<StaticComponent>("StaticComponent")
                .Base<IComponent>();
            MetaRegistry::Reflect<ArchetypeComponent>("ArchetypeComponent")
                .Base<IComponent>();

            MetaRegistry::Reflect<LocalTransformComponent>("LocalTransformComponent")
                .Base<IComponent>()
                .Property<&LocalTransformComponent::position>("position")
                .Property<&LocalTransformComponent::rotation>("rotation")
                .Property<&LocalTransformComponent::scale>("scale");
            MetaRegistry::Reflect<DerivedTransformComponent>("DerivedTransformComponent")
                .Base<IComponent>()
                .Property<&DerivedTransformComponent::position>("position", std::make_pair(PropertyAttribute::Serialize, false))
                .Property<&DerivedTransformComponent::rotation>("rotation", std::make_pair(PropertyAttribute::Serialize, false))
                .Property<&DerivedTransformComponent::scale>("scale", std::make_pair(PropertyAttribute::Serialize, false));
            MetaRegistry::Reflect<LocalToWorldComponent>("LocalToWorldComponent")
                .Base<IComponent>()
                .Property<&LocalToWorldComponent::local_to_world>("local_to_world", std::make_pair(PropertyAttribute::Serialize, false));
            MetaRegistry::Reflect<HierarchyComponent>("HierarchyComponent")
                .Base<IComponent>()
                .Property<&HierarchyComponent::parent>("parent", std::make_pair(PropertyAttribute::SpecialSerialize, PropertySpecialSerialize::EntityIdAsGuid))
                .Property<&HierarchyComponent::previous_sibling>("previous_sibling", std::make_pair(PropertyAttribute::SpecialSerialize, PropertySpecialSerialize::EntityIdAsGuid))
                .Property<&HierarchyComponent::next_sibling>("next_sibling", std::make_pair(PropertyAttribute::SpecialSerialize, PropertySpecialSerialize::EntityIdAsGuid))
                .Property<&HierarchyComponent::child_count>("child_count")
                .Property<&HierarchyComponent::first_child>("first_child", std::make_pair(PropertyAttribute::SpecialSerialize, PropertySpecialSerialize::EntityIdAsGuid))
                .Property<&HierarchyComponent::last_child>("last_child", std::make_pair(PropertyAttribute::SpecialSerialize, PropertySpecialSerialize::EntityIdAsGuid));

            MetaRegistry::Reflect<Physics::BoxColliderComponent>("BoxColliderComponent")
                .Base<IComponent>()
                .Property<&Physics::BoxColliderComponent::origin>("origin")
                .Property<&Physics::BoxColliderComponent::size>("size");
            MetaRegistry::Reflect<Physics::SphereColliderComponent>("SphereColliderComponent")
                .Base<IComponent>()
                .Property<&Physics::SphereColliderComponent::radius>("radius");

            MetaRegistry::Reflect<Rendering::CameraComponent>("CameraComponent")
                .Base<IComponent>();
            MetaRegistry::Reflect<Rendering::SpriteComponent>("SpriteComponent")
                .Base<IComponent>();
            MetaRegistry::Reflect<Rendering::MeshComponent>("MeshComponent")
                .Base<IComponent>();
            MetaRegistry::Reflect<Rendering::LocalMeshBoundsComponent>("LocalMeshBoundsComponent")
                .Base<IComponent>()
                .Property<&Rendering::LocalMeshBoundsComponent::bounds>("bounds");
            MetaRegistry::Reflect<Rendering::WorldMeshBoundsComponent>("WorldMeshBoundsComponent")
                .Base<IComponent>()
                .Property<&Rendering::WorldMeshBoundsComponent::bounds>("bounds", std::make_pair(PropertyAttribute::Serialize, false));
            MetaRegistry::Reflect<Rendering::DirectionalLightComponent>("DirectionalLightComponent")
                .Base<IComponent>();
            MetaRegistry::Reflect<Rendering::PointLightComponent>("PointLightComponent")
                .Base<IComponent>();
            MetaRegistry::Reflect<Rendering::SpotLightComponent>("SpotLightComponent")
                .Base<IComponent>();
        }
    }

    //--------------------------------------------------------------
    void WorldManager::Initialize() {
        ComponentRegistry::Initialize();
    }

    //--------------------------------------------------------------
    void WorldManager::Update() {
        HYP_PROFILE_SCOPE("WorldManager.Update");

        if (s_active_world) {
            // Transform
            {
                HYP_PROFILE_SCOPE("WorldManager.Update.Transform");

                HierarchyTransformSystem hierarchy_transform_system;
                hierarchy_transform_system.Run(s_active_world);

                LocalToWorldSystem local_to_world_system;
                local_to_world_system.Run(s_active_world);
            }

            // Physics
            {
                HYP_PROFILE_SCOPE("WorldManager.Update.Physics");

                Physics::UpdateColliderTransformSystem update_collider_transform_system;
                update_collider_transform_system.Run(s_active_world);
            }

            // Assets
            {
                HYP_PROFILE_SCOPE("WorldManager.Update.Assets");

                AssetLoadSystem asset_load_system;
                asset_load_system.Run(s_active_world);

                AssetUnloadSystem asset_unload_system;
                asset_unload_system.Run(s_active_world);
            }

            // Rendering
            {
                HYP_PROFILE_SCOPE("WorldManager.Update.Rendering");

                Rendering::MeshBoundsSystem bounds_system;
                bounds_system.Run(s_active_world);

                Rendering::EnvironmentSystem environment_system;
                environment_system.Run(s_active_world);

                Rendering::CameraSystem camera_system;
                camera_system.Run(s_active_world);

                Rendering::LightSystem light_system;
                light_system.Run(s_active_world);

                Rendering::SpriteSystem sprite_system;
                sprite_system.Run(s_active_world);

                Rendering::MeshSystem mesh_system;
                mesh_system.Run(s_active_world);
            }
        }
    }

    //--------------------------------------------------------------
    void WorldManager::Shutdown() {
        for (World *world : s_worlds) {
            delete world;
        }
        s_worlds.Clear();
    }

}