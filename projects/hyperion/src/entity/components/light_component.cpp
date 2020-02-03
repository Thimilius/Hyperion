#include "hyppch.hpp"

#include "hyperion/entity/components/light_component.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"

namespace Hyperion {

    void LightComponent::OnCreate() {
        GetEntity()->GetWorld()->AddLight(this);

        EntityComponent::OnCreate();
    }

    void LightComponent::OnDestroy() {
        GetEntity()->GetWorld()->RemoveLight(this);

        EntityComponent::OnCreate();
    }

}