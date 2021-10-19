//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/ui_systems.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/ecs/component/components/core_components.hpp"
#include "hyperion/ecs/component/components/ui_components.hpp"
#include "hyperion/ecs/world/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    //--------------------------------------------------------------
    void UILayoutSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("UILayoutSystem.Run");

        auto view = world->GetView<UIViewComponent>(ExcludeComponents<DisabledComponent>());
        for (EntityId entity : view) {
            UIViewComponent *ui_view = world->GetComponent<UIViewComponent>(entity);

            if (ui_view->root_element) {
                MeshBuilder mesh_builder;
                Rebuild(ui_view->root_element, mesh_builder);
            }
        }
    }

    //--------------------------------------------------------------
    void UILayoutSystem::Rebuild(UIElement *element, MeshBuilder &mesh_builder) {
        bool8 needs_rebuild = Display::HasChangedSize() ||element->IsDirty();
        if (needs_rebuild) {
            element->Rebuild(mesh_builder);
        }

        for (UIElement *child : element->GetHierarchy().GetChildren()) {
            Rebuild(child, mesh_builder);
        }
    }

    //--------------------------------------------------------------
    void UIEventSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("UIEventSystem.Run");

        auto view = world->GetView<UIViewComponent>(ExcludeComponents<DisabledComponent>());
        for (EntityId entity : view) {
            UIViewComponent *ui_view = world->GetComponent<UIViewComponent>(entity);
        }
    }

}