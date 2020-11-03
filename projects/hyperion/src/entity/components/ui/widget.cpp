#include "hyppch.hpp"

#include "hyperion/entity/components/ui/widget.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/ui/canvas.hpp"

namespace Hyperion::UI {

    // FIXME: We need a more reliable way to store the graphics in the parent canvas.
    // What happens if the parent changes or the graphic is created with no parent canvas?

    void Widget::OnCreate() {
        Component::OnCreate();

        Canvas *canvas = GetEntity()->GetComponentInParent<Canvas>();
        canvas->AddWidget(this);
        WidgetRegistry::RegisterWidget(canvas, this);
    }

    void Widget::OnDestroy() {
        Canvas *canvas = GetEntity()->GetComponentInParent<Canvas>();
        canvas->RemoveWidget(this);
        WidgetRegistry::UnregisterWidget(canvas, this);

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