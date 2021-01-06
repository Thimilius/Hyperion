#include "hyppch.hpp"

#include "hyperion/entity/components/behaviour.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/transform.hpp"

namespace Hyperion {

    bool Behaviour::IsActiveAndEnabled() {
        return m_enabled && GetTransform()->GetEntity()->IsActiveInHierarchy();
    }

}

HYP_REFLECT_REGISTER_BEGIN
{
    registration::class_<Behaviour>("Behaviour");
}
HYP_REFLECT_REGISTER_END
