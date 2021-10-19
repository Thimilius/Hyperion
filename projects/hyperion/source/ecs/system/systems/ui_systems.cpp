//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/ui_systems.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/core_components.hpp"
#include "hyperion/ecs/component/components/ui_components.hpp"
#include "hyperion/ecs/world/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    //--------------------------------------------------------------
    void UISystem::Run(World *world) {
        HYP_PROFILE_SCOPE("UISystem.Run");

        auto view = world->GetView<UIViewComponent>(ExcludeComponents<DisabledComponent>());
        for (EntityId entity : view) {
            UIViewComponent *ui_view = world->GetComponent<UIViewComponent>(entity);
        }
    }

}