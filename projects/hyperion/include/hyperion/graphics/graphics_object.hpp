#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsObjectDescription {
        String name = "";
    };

    class IGraphicsObject {
    public:
        virtual ~IGraphicsObject() = default;
    public:
        virtual const GraphicsObjectDescription &GetDescription() const = 0;
    };

}