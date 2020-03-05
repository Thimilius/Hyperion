#pragma once

#include "hyperion/entity/components/behaviour.hpp"

namespace Hyperion {

    class Collider : public Behaviour {
        HYP_OBJECT(Collider, Behaviour);
    };

}