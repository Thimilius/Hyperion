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
            .Base<IComponent>();
        MetaRegistry::Reflect<DerivedTransformComponent>("DerivedTransformComponent")
            .Base<IComponent>();
        MetaRegistry::Reflect<LocalToWorldComponent>("LocalToWorldComponent")
            .Base<IComponent>();
        MetaRegistry::Reflect<HierarchyComponent>("HierarchyComponent")
            .Base<IComponent>();

        MetaRegistry::Reflect<Physics::BoxColliderComponent>("BoxColliderComponent")
            .Base<IComponent>();
        MetaRegistry::Reflect<Physics::SphereColliderComponent>("SphereColliderComponent")
            .Base<IComponent>();

        MetaRegistry::Reflect<Rendering::CameraComponent>("CameraComponent")
            .Base<IComponent>();
        MetaRegistry::Reflect<Rendering::SpriteComponent>("SpriteComponent")
            .Base<IComponent>();
        MetaRegistry::Reflect<Rendering::RenderMeshComponent>("RenderMeshComponent")
            .Base<IComponent>();
        MetaRegistry::Reflect<Rendering::DirectionalLightComponent>("DirectionalLightComponent")
            .Base<IComponent>();
        MetaRegistry::Reflect<Rendering::PointLightComponent>("PointLightComponent")
            .Base<IComponent>();
        MetaRegistry::Reflect<Rendering::SpotLightComponent>("SpotLightComponent")
            .Base<IComponent>();
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

                Rendering::EnvironmentSystem environment_system;
                environment_system.Run(s_active_world);

                Rendering::CameraSystem camera_system;
                camera_system.Run(s_active_world);

                Rendering::LightSystem light_system;
                light_system.Run(s_active_world);

                Rendering::SpriteSystem sprite_system;
                sprite_system.Run(s_active_world);

                Rendering::RenderMeshSystem render_mesh_system;
                render_mesh_system.Run(s_active_world);
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