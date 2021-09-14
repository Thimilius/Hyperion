//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components.hpp"
#include "hyperion/ecs/system/asset_system.hpp"
#include "hyperion/ecs/system/transform_system.hpp"
#include "hyperion/ecs/system/render_system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    World *WorldManager::CreateWorld() {
        World *world = new World();
        s_worlds.Add(world);
        return world;
    }

    //--------------------------------------------------------------
    void WorldManager::DestroyWorld(World *world) {
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
        ComponentRegistry::Register<TagComponent>();
        ComponentRegistry::Register<TransformComponent>();
        ComponentRegistry::Register<LocalToWorldComponent>();
        ComponentRegistry::Register<HierarchyComponent>();
        ComponentRegistry::Register<CameraComponent>();
        ComponentRegistry::Register<SpriteComponent>();
        ComponentRegistry::Register<RenderMeshComponent>();
    }

    //--------------------------------------------------------------
    void WorldManager::Update() {
        if (s_active_world) {
            // Transformation
            {
                LocalToWorldSystem local_to_world_system;
                local_to_world_system.Run(s_active_world);
            }

            // Assets
            {
                AssetLoadSystem asset_load_system;
                asset_load_system.Run(s_active_world);

                AssetUnloadSystem asset_unload_system;
                asset_unload_system.Run(s_active_world);
            }

            // Rendering
            {
                Rendering::CameraSystem camera_system;
                camera_system.Run(s_active_world);

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