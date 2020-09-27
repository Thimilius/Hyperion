#include "hyppch.hpp"

#include "hyperion/entity/components/behaviour.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/transform.hpp"

namespace Hyperion {

    bool Behaviour::IsActiveAndEnabled() {
        return m_enabled && GetTransform()->GetEntity()->IsActiveInHierarchy();
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<Behaviour>(String(NAMEOF_SHORT_TYPE(Behaviour)));
}
