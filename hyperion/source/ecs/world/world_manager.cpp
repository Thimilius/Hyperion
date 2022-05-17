//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/components.hpp"
#include "hyperion/ecs/system/systems/physics_systems.hpp"
#include "hyperion/ecs/system/systems/transform_systems.hpp"
#include "hyperion/ecs/system/systems/render_systems.hpp"
#include "hyperion/ecs/system/systems/ui_systems.hpp"

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
  void WorldManager::Initialize() {
    
  }

  //--------------------------------------------------------------
  void WorldManager::Update() {
    HYP_PROFILE_SCOPE("WorldManager.Update");

    if (s_active_world) {
      EntityManager *entity_manager = s_active_world->GetEntityManager();
      
      // UI
      {
        HYP_PROFILE_SCOPE("WorldManager.Update.UI");

        UI::UIRebuildSystem ui_rebuild_system;
        ui_rebuild_system.Run(entity_manager);

        UI::UIEventSystem ui_event_system;
        ui_event_system.Run(entity_manager);
      }

      // Transform
      {
        HYP_PROFILE_SCOPE("WorldManager.Update.Transform");

        HierarchyTransformSystem hierarchy_transform_system;
        hierarchy_transform_system.Run(entity_manager);

        LocalToWorldSystem local_to_world_system;
        local_to_world_system.Run(entity_manager);
      }

      // Physics
      {
        HYP_PROFILE_SCOPE("WorldManager.Update.Physics");

        Physics::UpdateColliderTransformSystem update_collider_transform_system;
        update_collider_transform_system.Run(entity_manager);
      }

      // Rendering
      {
        HYP_PROFILE_SCOPE("WorldManager.Update.Rendering");

        Rendering::MeshBoundsSystem bounds_system;
        bounds_system.Run(entity_manager);

        Rendering::EnvironmentSystem environment_system;
        environment_system.Run(entity_manager);

        Rendering::CameraSystem camera_system;
        camera_system.Run(entity_manager);

        Rendering::LightSystem light_system;
        light_system.Run(entity_manager);

        Rendering::SpriteRenderSystem sprite_render_system;
        sprite_render_system.Run(entity_manager);

        Rendering::MeshRenderSystem mesh_render_system;
        mesh_render_system.Run(entity_manager);

        Rendering::UIRenderSystem ui_render_system;
        ui_render_system.Run(entity_manager);
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
