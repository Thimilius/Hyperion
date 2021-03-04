//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/world_manager.hpp"

//---------------------- Project Includes ----------------------
#include <hyperion/core/serialization/serializer.hpp>
#include "hyperion/entity/entity.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    World *WorldManager::CreateWorld(const String &name) {
        World *world = new World(name);
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

        Object::Destroy(world);
    }

    //--------------------------------------------------------------
    void WorldManager::Shutdown() {
        for (World *world : s_worlds) {
            Object::DestroyImmediate(world);
        }
    }

    //--------------------------------------------------------------
    String WorldManager::SerializeWorld(World *world) {
        return Serializer::Serialize(world);
    }

    //--------------------------------------------------------------
    World *WorldManager::DeserializeWorld(const String &text) {
        World *world = Serializer::DeserializeObject<World>(text, []() { return WorldManager::CreateWorld(); });
        world->OnAfterDeserialization();
        return world;
    }

}