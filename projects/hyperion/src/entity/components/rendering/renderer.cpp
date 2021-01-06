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

HYP_REFLECT_REGISTER_BEGIN
{
    registration::class_<Renderer>("Renderer");
}
HYP_REFLECT_REGISTER_END
