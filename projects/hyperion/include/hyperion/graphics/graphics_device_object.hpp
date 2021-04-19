#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsDeviceObjectDescription {
        String name = "";
    };

    class GraphicsDeviceObject {
    public:
        virtual ~GraphicsDeviceObject() = default;
    public:
        virtual const GraphicsDeviceObjectDescription &GetDescription() const = 0;
    };

}