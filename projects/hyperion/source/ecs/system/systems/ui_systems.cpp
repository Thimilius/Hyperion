//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/ui_systems.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/app/input.hpp"
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

            if (ui_view->root_element) {
                UIViewComponent::State state = ui_view->state;

                Array<UIElement *> hovered_elements;
                RaycastElements(ui_view->root_element, Input::GetMousePosition(), hovered_elements);
                

                if (!hovered_elements.IsEmpty()) {
                    // The array we get back will be depth sorted in reverse.
                    // This means the top most hovered element we are interested in is at the end.
                    UIElement *hovered_element = hovered_elements.GetLast();

                    if (state.hovered_element) {
                        // POINTER EXIT
                        state.hovered_element->GetRenderer().color = Color::White();
                        state.hovered_element->GetStyle().SetOpacity(1.0f);
                    }
                    state.hovered_element = hovered_element;

                    // POINTER ENTER
                    state.hovered_element->GetRenderer().color = Color::Grey();
                    state.hovered_element->GetStyle().SetOpacity(0.5f);
                } else {
                    if (state.hovered_element) {
                        // POINTER EXIT
                        state.hovered_element->GetRenderer().color = Color::White();
                        state.hovered_element->GetStyle().SetOpacity(1.0f);

                        state.hovered_element = nullptr;
                    }
                }

                if (Input::IsMouseButtonUp(MouseButtonCode::Left)) {
                    if (state.hovered_element) {
                        // CLICK
                    }
                }

                ui_view->state = state;
            }
        }
    }

    //--------------------------------------------------------------
    void UIEventSystem::RaycastElements(UIElement *element, Vector2 screen_point, Array<UIElement *> &raycasted) {
        if (element->ContainsScreenPoint(screen_point)) {
            raycasted.Add(element);
        }

        for (UIElement *child : element->GetHierarchy().GetChildren()) {
            RaycastElements(child, screen_point, raycasted);
        }
    }

}