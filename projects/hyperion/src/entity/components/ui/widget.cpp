#include "hyppch.hpp"

#include "hyperion/entity/components/ui/widget.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/ui/canvas.hpp"

namespace Hyperion::UI {

    // FIXME: We need a more reliable way to store the graphics in the parent canvas.
    // What happens if the parent changes or the graphic is created with no parent canvas?

    void Widget::OnCreate() {
        Component::OnCreate();

        GetEntity()->GetComponentInParent<Canvas>()->AddWidget(this);
    }

    void Widget::OnDestroy() {
        GetEntity()->GetComponentInParent<Canvas>()->RemoveWidget(this);

        Component::OnDestroy();
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;
    using namespace Hyperion::UI;

    registration::class_<Widget>(HYP_NAME_OF_TYPE(Widget));
}