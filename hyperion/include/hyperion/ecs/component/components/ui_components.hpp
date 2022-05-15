//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ui/ui_element.hpp"
#include "hyperion/ui/ui_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  struct UIViewComponent {
    UIElement *root_element = nullptr;

    ScalingMode scaling_mode = ScalingMode::ScaleWithScreenSize;
    Vector2 reference_resolution = Vector2(1280, 720);

    struct State {
      UIElement *hovered_element = nullptr;
      UIElement *pressed_element = nullptr;
      UIElement *selected_element = nullptr;
    } state;
  };

}
