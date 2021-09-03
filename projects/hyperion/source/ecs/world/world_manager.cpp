//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components.hpp"

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

        s_worlds.Remove(world);
        delete world;
    }

    //--------------------------------------------------------------
    void WorldManager::Initialize() {
        ComponentRegistry::Register<TagComponent>();
        ComponentRegistry::Register<TransformComponent>();
        ComponentRegistry::Register<LocalToWorldComponent>();
        ComponentRegistry::Register<HierarchyComponent>();
    }

    //--------------------------------------------------------------
    void WorldManager::Shutdown() {
        for (World *world : s_worlds) {
            delete world;
        }
        s_worlds.Clear();
    }

}