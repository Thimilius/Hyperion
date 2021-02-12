//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/world_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    World *WorldManager::CreateWorld() {
        World *world = new World("World");
        s_worlds.push_back(world);
        return world;
    }

    //--------------------------------------------------------------
    void WorldManager::DestroyWorld(World *world) {
        HYP_ASSERT_MESSAGE(s_active_world != world, "Can not destroy active world!");

        auto begin = s_worlds.begin();
        auto end = s_worlds.end();
        if (std::find(begin, end, world) != end) {
            s_worlds.erase(std::remove(begin, end, world));
        }

        delete world;
    }

    //--------------------------------------------------------------
    void WorldManager::Shutdown() {
        for (World *world : s_worlds) {
            delete world;
        }
    }

}