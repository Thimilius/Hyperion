#pragma once

#include "hyperion/common.hpp"
#include "hyperion/math/vec3.hpp"

namespace Hyperion::Math {

    struct SBounds {
        SVec3 center;
        SVec3 extends;

        SBounds();
        SBounds(SVec3 center, SVec3 extends);

        SVec3 GetSize() const;
        SVec3 GetMin() const;
        SVec3 GetMax() const;

        void SetMinMax(SVec3 min, SVec3 max);

        TString ToString() const;
    };

}