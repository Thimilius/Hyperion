#include "hyppch.hpp"

#include "hyperion/entity/components/rendering/light.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"

namespace Hyperion {

    void Light::OnCreate() {
        Behaviour::OnCreate();

        GetEntity()->GetWorld()->AddLight(this);
    }

    void Light::OnDestroy() {
        GetEntity()->GetWorld()->RemoveLight(this);

        Behaviour::OnDestroy();
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<Light>(String(NAMEOF_SHORT_TYPE(Light)));
}
