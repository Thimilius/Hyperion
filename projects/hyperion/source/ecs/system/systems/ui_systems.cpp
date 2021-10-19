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
    void UIRebuildSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("UIRebuildSystem.Run");

        auto view = world->GetView<UIViewComponent>(ExcludeComponents<DisabledComponent>());
        for (EntityId entity : view) {
            UIViewComponent *ui_view = world->GetComponent<UIViewComponent>(entity);

            if (ui_view->root_element) {
                float32 ui_scale = 1.0f;
                switch (ui_view->scaling_mode) {
                    case UIScalingMode::ScaleWithScreenSize:
                    {
                        float32 display_width = static_cast<float32>(Display::GetWidth());
                        float32 display_height = static_cast<float32>(Display::GetHeight());

                        float32 log_base = 2;
                        float32 log_width = Math::Log(display_width / ui_view->reference_resolution.x) / Math::Log(log_base);
                        float32 log_height = Math::Log(display_height / ui_view->reference_resolution.y) / Math::Log(log_base);
                        float32 log_weighted_average = Math::Lerp(log_width, log_height, 0.5f);
                        float32 computed_scale = Math::Pow(log_base, log_weighted_average);
                        ui_scale = computed_scale;
                        break;
                    }
                    case UIScalingMode::ConstantPixelSize:
                    {
                        ui_scale = 1.0f;
                        break;
                    }
                    default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
                }

                MeshBuilder mesh_builder;
                Rebuild(ui_view->root_element, ui_scale, mesh_builder);
            }
        }
    }

    //--------------------------------------------------------------
    void UIRebuildSystem::Rebuild(UIElement *element, float32 ui_scale, MeshBuilder &mesh_builder) {
        bool8 needs_rebuild = Display::HasChangedSize() ||element->IsDirty();
        if (needs_rebuild) {
            element->Rebuild(ui_scale, mesh_builder);
        }

        for (UIElement *child : element->GetHierarchy().GetChildren()) {
            Rebuild(child, ui_scale, mesh_builder);
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