//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/components.hpp"
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
        // Basic component types
        {
            MetaRegistry::Reflect<Rendering::LayerMask>("LayerMask", std::make_pair(TypeAttribute::EnumFlags, true))
                .Property<Rendering::LayerMask::Nothing>("Nothing")
                .Property<Rendering::LayerMask::Layer1>("Layer1")
                .Property<Rendering::LayerMask::Layer2>("Layer2")
                .Property<Rendering::LayerMask::Layer3>("Layer3")
                .Property<Rendering::LayerMask::Layer4>("Layer4")
                .Property<Rendering::LayerMask::Layer5>("Layer5")
                .Property<Rendering::LayerMask::Layer6>("Layer6")
                .Property<Rendering::LayerMask::Layer7>("Layer7")
                .Property<Rendering::LayerMask::Layer8>("Layer8")
                .Property<Rendering::LayerMask::Layer9>("Layer9")
                .Property<Rendering::LayerMask::Layer10>("Layer10")
                .Property<Rendering::LayerMask::Layer11>("Layer11")
                .Property<Rendering::LayerMask::Layer12>("Layer12")
                .Property<Rendering::LayerMask::Layer13>("Layer13")
                .Property<Rendering::LayerMask::Layer14>("Layer14")
                .Property<Rendering::LayerMask::Layer15>("Layer15")
                .Property<Rendering::LayerMask::Layer16>("Layer16")
                .Property<Rendering::LayerMask::Layer17>("Layer17")
                .Property<Rendering::LayerMask::Layer18>("Layer18")
                .Property<Rendering::LayerMask::Layer19>("Layer19")
                .Property<Rendering::LayerMask::Layer20>("Layer20")
                .Property<Rendering::LayerMask::Layer21>("Layer21")
                .Property<Rendering::LayerMask::Layer22>("Layer22")
                .Property<Rendering::LayerMask::Layer23>("Layer23")
                .Property<Rendering::LayerMask::Layer24>("Layer24")
                .Property<Rendering::LayerMask::Layer25>("Layer25")
                .Property<Rendering::LayerMask::Layer26>("Layer26")
                .Property<Rendering::LayerMask::Layer27>("Layer27")
                .Property<Rendering::LayerMask::Layer28>("Layer28")
                .Property<Rendering::LayerMask::Layer29>("Layer29")
                .Property<Rendering::LayerMask::Layer30>("Layer30")
                .Property<Rendering::LayerMask::Layer31>("Layer31")
                .Property<Rendering::LayerMask::Everything>("Everything");

            MetaRegistry::Reflect<Rendering::CameraProjectionMode>("CameraProjectionMode")
                .Property<Rendering::CameraProjectionMode::Perspective>("Perspective")
                .Property<Rendering::CameraProjectionMode::Orthographic>("Orthographic");

            MetaRegistry::Reflect<Rendering::CameraClearMode>("CameraClearMode")
                .Property<Rendering::CameraClearMode::Nothing>("Nothing")
                .Property<Rendering::CameraClearMode::Depth>("Depth")
                .Property<Rendering::CameraClearMode::Color>("Color")
                .Property<Rendering::CameraClearMode::Skybox>("Skybox");

            MetaRegistry::Reflect<Rendering::CameraViewportClipping>("CameraViewportClipping")
                .Property<&Rendering::CameraViewportClipping::x>("x")
                .Property<&Rendering::CameraViewportClipping::y>("y")
                .Property<&Rendering::CameraViewportClipping::width>("width")
                .Property<&Rendering::CameraViewportClipping::height>("height");
        }

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
                .Base<IComponent>()
                .Property<&Rendering::CameraComponent::projection_mode>("projection_mode")
                .Property<&Rendering::CameraComponent::clear_mode>("clear_mode")
                .Property<&Rendering::CameraComponent::background_color>("background_color")
                .Property<&Rendering::CameraComponent::culling_mask>("culling_mask")
                .Property<&Rendering::CameraComponent::near_plane>("near_plane")
                .Property<&Rendering::CameraComponent::far_plane>("far_plane")
                .Property<&Rendering::CameraComponent::fov>("fov")
                .Property<&Rendering::CameraComponent::orthographic_size>("orthographic_size")
                .Property<&Rendering::CameraComponent::viewport_clipping>("viewport_clipping")
                .Property<&Rendering::CameraComponent::view_matrix>("view_matrix", std::make_pair(PropertyAttribute::Serialize, false))
                .Property<&Rendering::CameraComponent::projection_matrix>("projection_matrix", std::make_pair(PropertyAttribute::Serialize, false))
                .Property<&Rendering::CameraComponent::view_projection_matrix>("view_projection_matrix", std::make_pair(PropertyAttribute::Serialize, false))
                .Property<&Rendering::CameraComponent::inverse_view_matrix>("inverse_view_matrix", std::make_pair(PropertyAttribute::Serialize, false))
                .Property<&Rendering::CameraComponent::inverse_projection_matrix>("inverse_projection_matrix", std::make_pair(PropertyAttribute::Serialize, false))
                .Property<&Rendering::CameraComponent::inverse_view_projection_matrix>("inverse_view_projection_matrix", std::make_pair(PropertyAttribute::Serialize, false));
            MetaRegistry::Reflect<Rendering::SpriteComponent>("SpriteComponent")
                .Base<IComponent>()
                .Property<&Rendering::SpriteComponent::color>("color");
            MetaRegistry::Reflect<Rendering::MeshComponent>("MeshComponent")
                .Base<IComponent>()
                .Property<&Rendering::MeshComponent::mesh>("mesh", std::make_pair(PropertyAttribute::SpecialSerialize, PropertySpecialSerialize::PointerAsAssetGuid))
                .Property<&Rendering::MeshComponent::sub_mesh_index>("sub_mesh_index")
                .Property<&Rendering::MeshComponent::material>("material", std::make_pair(PropertyAttribute::SpecialSerialize, PropertySpecialSerialize::PointerAsAssetGuid))
                .Property<&Rendering::MeshComponent::layer_mask>("layer_mask");
            MetaRegistry::Reflect<Rendering::LocalMeshBoundsComponent>("LocalMeshBoundsComponent")
                .Base<IComponent>()
                .Property<&Rendering::LocalMeshBoundsComponent::bounds>("bounds");
            MetaRegistry::Reflect<Rendering::WorldMeshBoundsComponent>("WorldMeshBoundsComponent")
                .Base<IComponent>()
                .Property<&Rendering::WorldMeshBoundsComponent::bounds>("bounds", std::make_pair(PropertyAttribute::Serialize, false));
            MetaRegistry::Reflect<Rendering::LightComponent>("LightComponent")
                .Base<IComponent>()
                .Property<&Rendering::LightComponent::intensity>("intensity")
                .Property<&Rendering::LightComponent::color>("color");
            MetaRegistry::Reflect<Rendering::DirectionalLightComponent>("DirectionalLightComponent")
                .Base<Rendering::LightComponent>();
            MetaRegistry::Reflect<Rendering::PointLightComponent>("PointLightComponent")
                .Base<Rendering::LightComponent>()
                .Property<&Rendering::PointLightComponent::range>("range");
            MetaRegistry::Reflect<Rendering::SpotLightComponent>("SpotLightComponent")
                .Base<Rendering::LightComponent>()
                .Property<&Rendering::SpotLightComponent::inner_spot_radius>("inner_spot_radius")
                .Property<&Rendering::SpotLightComponent::outer_spot_radius>("outer_spot_radius");
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