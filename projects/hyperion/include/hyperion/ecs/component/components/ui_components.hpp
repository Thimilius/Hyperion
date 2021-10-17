#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component.hpp"
#include "hyperion/ui/ui_element.hpp"
#include "hyperion/ui/ui_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    struct UIViewComponent : public IComponent {
        UIElement *root_element = nullptr;

        UIScalingMode scaling_mode = UIScalingMode::ScaleWithScreenSize;
        Vector2 reference_resolution = Vector2(1280, 720);
    };

}