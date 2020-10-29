#include "hyppch.hpp"

#include "hyperion/entity/components/ui/graphic.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/ui/canvas.hpp"

namespace Hyperion::UI {

    // FIXME: We need a more reliable way to store the graphics in the parent canvas.
    // What happens if the parent changes or the graphic is created with no parent canvas?

    void Graphic::OnCreate() {
        Component::OnCreate();

        GetEntity()->GetComponentInParent<Canvas>()->AddUIGraphic(this);
    }

    void Graphic::OnDestroy() {
        GetEntity()->GetComponentInParent<Canvas>()->RemoveUIGraphic(this);

        Component::OnDestroy();
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;
    using namespace Hyperion::UI;

    registration::class_<Graphic>(HYP_NAME_OF_TYPE(Graphic));
}