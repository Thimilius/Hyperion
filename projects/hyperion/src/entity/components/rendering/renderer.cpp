#include "hyppch.hpp"

#include "hyperion\entity\components\rendering\renderer.hpp"

namespace Hyperion {

    void Renderer::OnCreate() {
        Behaviour::OnCreate();
    }

    void Renderer::OnDestroy() {
        Behaviour::OnDestroy();
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<Renderer>(String(NAMEOF_SHORT_TYPE(Renderer)));
}
