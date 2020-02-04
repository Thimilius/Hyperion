#pragma once

#include "hyperion/rendering/camera.hpp"

namespace Hyperion::Rendering {

    class RenderPipeline {
    public:
        virtual ~RenderPipeline() = default;

        virtual void Render(const Ref<Camera> &camera) = 0;
    };

}