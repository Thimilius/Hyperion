#include "hyppch.hpp"

#include "hyperion/entity/world_manager.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"

namespace Hyperion {

    World *WorldManager::CreateWorld() {
        World *world = new World();
        s_worlds.push_back(world);
        return world;
    }

    void WorldManager::DestroyWorld(World *world) {
        HYP_ASSERT_MESSAGE(s_active_world != world, "Can not destroy active world!");

        auto begin = s_worlds.begin();
        auto end = s_worlds.end();
        if (std::find(begin, end, world) != end) {
            s_worlds.erase(std::remove(begin, end, world));
        }

        delete world;
    }

    void WorldManager::Init(EntitySettings settings) {
        World *world = settings.start_world;
        if (!world) {
            // We are currently forcing a new world with a camera
            world = WorldManager::CreateWorld();
            Entity::Create("Camera", Vec3(), Quaternion::Identity(), nullptr, world)->AddComponent<Camera>();
        }
        WorldManager::SetActiveWorld(world);
    }

    void WorldManager::Shutdown() {
        for (World *world : s_worlds) {
            delete world;
        }
    }

}