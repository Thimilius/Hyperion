#pragma once

#include "hyperion/rendering/camera_data.hpp"

namespace Hyperion::Rendering {

    class RenderPipeline {
    public:
        virtual ~RenderPipeline() = default;

        virtual void Render(const CameraData &camera) = 0;
    };

}