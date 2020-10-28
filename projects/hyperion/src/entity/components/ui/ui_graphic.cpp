#include "hyppch.hpp"

#include "hyperion/entity/components/ui/ui_graphic.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/ui/ui_canvas.hpp"

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<UIGraphic>(HYP_NAME_OF_TYPE(UIGraphic));
}