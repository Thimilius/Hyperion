#include "hyppch.hpp"

#include "hyperion/entity/components/rendering/light.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"

namespace Hyperion {

    void Light::OnCreate() {
        Behaviour::OnCreate();

        GetWorld()->AddLight(this);
    }

    void Light::OnDestroy() {
        GetWorld()->RemoveLight(this);

        Behaviour::OnDestroy();
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<Light>(HYP_NAME_OF_TYPE(Light));
}
