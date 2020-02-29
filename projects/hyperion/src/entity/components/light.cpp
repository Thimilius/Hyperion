#include "hyppch.hpp"

#include "hyperion/entity/components/light.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"

namespace Hyperion {

    void Light::OnCreate() {
        GetEntity()->GetWorld()->AddLight(this);

        Component::OnCreate();
    }

    void Light::OnDestroy() {
        GetEntity()->GetWorld()->RemoveLight(this);

        Component::OnCreate();
    }

}