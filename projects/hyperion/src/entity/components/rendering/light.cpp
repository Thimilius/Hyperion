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

HYP_REFLECT_REGISTER_BEGIN
{
    registration::class_<Light>("Light");
}
HYP_REFLECT_REGISTER_END
