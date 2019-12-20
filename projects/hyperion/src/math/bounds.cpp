#include "hyppch.hpp"

#include "hyperion/math/bounds.hpp"

namespace Hyperion::Math {

    SBounds::SBounds() { }
    
    SBounds::SBounds(SVec3 center, SVec3 size)
        : center(center), extends(size * 0.5f) { }

    SVec3 SBounds::GetSize() const {
        return extends * 2.0f;
    }

    SVec3 SBounds::GetMin() const {
        return center - extends;
    }

    SVec3 SBounds::GetMax() const {
        return center + extends;
    }

    void SBounds::SetMinMax(SVec3 min, SVec3 max) {
        extends = (max - min) * 0.5f;
        center = min + extends;
    }

    TString SBounds::ToString() const {
        return CStringUtils::Format("(Center: {}, Extends: {})", center.ToString(), extends.ToString());
    }

}
