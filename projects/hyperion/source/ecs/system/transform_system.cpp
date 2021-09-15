//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/transform_system.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/transform_components.hpp"
#include "hyperion/ecs/world/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void LocalToWorldSystem::Run(World *world) {
        auto view = world->GetView<LocalTransformComponent, LocalToWorldComponent>();
        for (EntityId entity : view) {
            LocalTransformComponent *local_transform = world->GetComponent<LocalTransformComponent>(entity);
            LocalToWorldComponent *local_to_world = world->GetComponent<LocalToWorldComponent>(entity);
            local_to_world->local_to_world = Matrix4x4::TRS(local_transform->position, local_transform->rotation, local_transform->scale);
        }
    }

}