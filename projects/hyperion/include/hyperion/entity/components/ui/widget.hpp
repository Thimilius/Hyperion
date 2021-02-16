#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/behaviour.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Widget : public Behaviour {
        HYP_REFLECT(Behaviour);
    };

}