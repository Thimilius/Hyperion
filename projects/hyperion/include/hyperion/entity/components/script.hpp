#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/component.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Entity;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Script final : public Component {
        HYP_REFLECT();
    };

}