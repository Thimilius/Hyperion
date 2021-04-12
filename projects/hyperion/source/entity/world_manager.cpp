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

        delete world;
    }

    //--------------------------------------------------------------
    void WorldManager::Initialize() {
#if HYP_EDITOR
        s_editor_world = CreateWorld("Editor World");
#endif
    }

    //--------------------------------------------------------------
    void WorldManager::Update(float32 delta_time) {
#if HYP_EDITOR
        s_editor_world->OnUpdate(delta_time);
#endif

        s_active_world->OnUpdate(delta_time);
    }

    //--------------------------------------------------------------
    void WorldManager::LateUpdate(float32 delta_time) {
#if HYP_EDITOR
        s_editor_world->OnLateUpdate(delta_time);
#endif

        s_active_world->OnLateUpdate(delta_time);
    }

    //--------------------------------------------------------------
    void WorldManager::Shutdown() {
        for (World *world : s_worlds) {
            delete world;
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