#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/non_copyable.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class GraphicsDeviceContext : public INonCopyable {
    public:
        virtual ~GraphicsDeviceContext() = default;
    };

}