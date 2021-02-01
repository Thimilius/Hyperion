#include "hyppch.hpp"

#include "hyperion/entity/world_manager.hpp"

#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    World *WorldManager::CreateWorld() {
        World *world = new World("World");
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

        world->OnDestroy();
        delete world;
    }

    void WorldManager::Shutdown() {
        for (World *world : s_worlds) {
            world->OnDestroy();
            delete world;
        }
    }


    void WorldManager::Update(float32 delta_time) {
        for (auto &[world, components] : s_components_to_update) {
            for (Component *component : components) {
                component->OnUpdate(delta_time);
            }
        }
    }

    void WorldManager::RegisterComponentForUpdate(Component *component) {
        s_components_to_update[component->GetWorld()].insert(component);
    }

    void WorldManager::UnregisterComponentForUpdate(Component *component) {
        World *world = component->GetWorld();

        if (s_components_to_update.find(world) != s_components_to_update.end()) {
            Set<Component *> &components = s_components_to_update.at(world);
            auto begin = components.begin();
            auto end = components.end();
            if (std::find(begin, end, component) != end) {
                components.erase(component);
            }
        }
    }

}