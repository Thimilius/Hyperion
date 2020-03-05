#pragma once

#include "hyperion/entity/components/physics/collider.hpp"

namespace Hyperion {

    class BoxCollider : public Collider {
        HYP_OBJECT(BoxCollider, Collider);
    }

}