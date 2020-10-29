#include "hyppch.hpp"

#include "hyperion/entity/components/ui/ui_graphic.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/ui/ui_canvas.hpp"

namespace Hyperion {

    // FIXME: We need a more reliable way to store the graphics in the parent canvas.
    // What happens if the parent changes or the graphic is created with no parent canvas?

    void UIGraphic::OnCreate() {
        Component::OnCreate();

        GetEntity()->GetComponentInParent<UICanvas>()->AddUIGraphic(this);
    }

    void UIGraphic::OnDestroy() {
        GetEntity()->GetComponentInParent<UICanvas>()->RemoveUIGraphic(this);

        Component::OnDestroy();
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<UIGraphic>(HYP_NAME_OF_TYPE(UIGraphic));
}