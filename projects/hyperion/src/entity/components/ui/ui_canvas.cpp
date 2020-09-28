#include "hyppch.hpp"

#include "hyperion/entity/components/ui/ui_canvas.h"

#include "hyperion/entity/world.hpp"

namespace Hyperion {

    void UICanvas::OnCreate() {
        Component::OnCreate();

        GetWorld()->AddUICanvas(this);
    }

    void UICanvas::OnDestroy() {
        GetWorld()->RemoveUICanvas(this);

        Component::OnDestroy();
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<UICanvas>(HYP_NAME_OF_TYPE(UICanvas));
}