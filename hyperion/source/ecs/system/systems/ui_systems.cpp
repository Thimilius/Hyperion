//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/ui_systems.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/ecs/component/components/core_components.hpp"
#include "hyperion/ecs/component/components/ui_components.hpp"
#include "hyperion/ecs/world/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  //--------------------------------------------------------------
  void UIRebuildSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("UIRebuildSystem.Run");

    auto view = manager->GetView<UIViewComponent>();
    for (EntityId entity : view) {
      UIViewComponent *ui_view = manager->GetComponent<UIViewComponent>(entity);

      Run(ui_view);
    }
  }

  //--------------------------------------------------------------
  void UIRebuildSystem::Run(UIViewComponent *ui_view) {
    if (ui_view->root_element) {
      float32 ui_scale = 1.0f;
      switch (ui_view->scaling_mode) {
        case ScalingMode::ScaleWithScreenSize: {
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
        case ScalingMode::ConstantPixelSize: {
          ui_scale = 1.0f;
          break;
        }
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
          break;
      }

      MeshBuilder mesh_builder;
      Rebuild(ui_view->root_element, ui_scale, mesh_builder);
    }
  }

  //--------------------------------------------------------------
  void UIRebuildSystem::Rebuild(UIElement *element, float32 ui_scale, MeshBuilder &mesh_builder) {
    bool8 needs_rebuild = Display::HasChangedSize() || element->IsDirty();
    if (needs_rebuild) {
      element->Rebuild(ui_scale, mesh_builder);
    }

    for (UIElement *child : element->GetHierarchy().GetChildren()) {
      Rebuild(child, ui_scale, mesh_builder);
    }
  }

  //--------------------------------------------------------------
  void UIEventSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("UIEventSystem.Run");

    auto view = manager->GetView<UIViewComponent>();
    for (EntityId entity : view) {
      UIViewComponent *ui_view = manager->GetComponent<UIViewComponent>(entity);

      Run(ui_view);
    }
  }

  //--------------------------------------------------------------
  void UIEventSystem::Run(UIViewComponent *ui_view) {
    if (ui_view->root_element) {
      UIViewComponent::State state = ui_view->state;

      Array<UIElement *> hovered_elements;
      RaycastElements(ui_view->root_element, Input::GetMousePosition(), hovered_elements);

      if (!hovered_elements.IsEmpty()) {
        // The array we get back will be depth sorted in reverse.
        // This means the top most hovered element we are interested in is at the end.
        UIElement *hovered_element = hovered_elements.GetLast();

        if (state.hovered_element != hovered_element) {
          if (state.hovered_element) {
            SendEvent(state.hovered_element, UIEventType::PointerExit);
          }

          state.hovered_element = hovered_element;
          SendEvent(state.hovered_element, UIEventType::PointerEnter);
        }
      } else {
        if (state.hovered_element) {
          SendEvent(state.hovered_element, UIEventType::PointerExit);
          state.hovered_element = nullptr;
        }

        if (state.selected_element) {
          if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
            SendEvent(state.selected_element, UIEventType::Deselect);
            state.selected_element = nullptr;
          }
        }
      }

      if (state.pressed_element) {
        if (Input::IsMouseButtonUp(MouseButtonCode::Left)) {
          SendEvent(state.pressed_element, UIEventType::PointerUp);

          // We only send the click to the button we are hovering and have pressed.
          if (state.hovered_element == state.pressed_element) {
            SendEvent(state.hovered_element, UIEventType::PointerClick);

            if (state.hovered_element != state.selected_element) {
              state.selected_element = state.hovered_element;

              SendEvent(state.selected_element, UIEventType::Select);
            }
          }

          state.pressed_element = nullptr;
        }
      }

      if (state.hovered_element) {
        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
          SendEvent(state.hovered_element, UIEventType::PointerDown);
          state.pressed_element = state.hovered_element;

          if (state.selected_element) {
            if (state.selected_element != state.hovered_element) {
              SendEvent(state.selected_element, UIEventType::Deselect);
              state.selected_element = nullptr;
            }
          }
        }
        if (Input::IsMouseButtonUp(MouseButtonCode::Left)) {
          SendEvent(state.hovered_element, UIEventType::PointerUp);
          state.pressed_element = nullptr;
        }

        if (Input::HasMouseMoved()) {
          SendEvent(state.hovered_element, UIEventType::PointerMove);
        }
        if (Input::HasMouseScrolled()) {
          SendEvent(state.hovered_element, UIEventType::PointerScroll);
        }
      }

      if (state.selected_element) {
        SendEvent(state.selected_element, UIEventType::SelectUpdate);

        if (Input::IsKeyDown(KeyCode::Return)) {
          SendEvent(state.selected_element, UIEventType::Submit);
        }
        if (Input::IsKeyDown(KeyCode::Escape)) {
          SendEvent(state.selected_element, UIEventType::Cancel);
        }

        for (AppEvent *app_event : Input::GetEvents()) {
          if (app_event->GetType() == AppEventType::KeyPressed) {
            KeyPressedAppEvent *event = static_cast<KeyPressedAppEvent *>(app_event);
            SendEvent(state.selected_element, UIEventType::KeyPressed, event->GetKeyCode(), event->GetKeyModifier());
          } else if (app_event->GetType() == AppEventType::KeyTyped) {
            KeyTypedAppEvent *event = static_cast<KeyTypedAppEvent *>(app_event);
            SendEvent(state.selected_element, UIEventType::KeyTyped, event->GetKeyCode(), event->GetKeyModifier(), event->GetCharacter());
          }
        }
      }

      ui_view->state = state;
    }
  }

  //--------------------------------------------------------------
  void UIEventSystem::RaycastElements(UIElement *element, Vector2Int screen_point, Array<UIElement *> &raycasted) {
    if (element->IsRaycastTarget() && element->ContainsScreenPoint(screen_point.ToFloat())) {
      raycasted.Add(element);
    }

    for (UIElement *child : element->GetHierarchy().GetChildren()) {
      RaycastElements(child, screen_point, raycasted);
    }
  }

  //--------------------------------------------------------------
  void UIEventSystem::SendEvent(UIElement *element, UIEventType type, KeyCode key_code, KeyModifier key_modifier, const String &key_typed) {
    UIEvent event;
    event.m_type = type;
    event.m_pointer_position = Input::GetMousePosition(); // FIXME: This is in the wrong coordinate space!
    event.m_pointer_scroll = Input::GetMouseScroll();
    event.m_key_code = key_code;
    event.m_key_modifier = key_modifier;
    event.m_key_typed = key_typed;
    element->OnEvent(event);
  }

}
