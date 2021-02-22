 #pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/component.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Canvas : public Component {
        HYP_REFLECT(Component);
    private:
        Canvas() : Component("Canvas") { }
    private:
        static Canvas *Create();
    };

}