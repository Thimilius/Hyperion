#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class VSyncMode {
        DontSync,
        EveryVBlank,
        EverySecondVBlank
    };

    struct RenderDriverContextProperties {
        String vendor;
        String renderer;
        String version;
    };

    struct RenderDriverContextDescriptor {
        int32 color_bits = 32;
        int32 depth_bits = 24;
        int32 stencil_bits = 8;
        int32 msaa_samples = 0;
    };

}