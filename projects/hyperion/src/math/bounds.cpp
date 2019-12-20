#include "hyppch.hpp"

#include "hyperion/math/bounds.hpp"

namespace Hyperion {

    Bounds::Bounds() { }
    
    Bounds::Bounds(Vec3 center, Vec3 size)
        : center(center), extends(size * 0.5f) { }

    Vec3 Bounds::GetSize() const {
        return extends * 2.0f;
    }

    Vec3 Bounds::GetMin() const {
        return center - extends;
    }

    Vec3 Bounds::GetMax() const {
        return center + extends;
    }

    void Bounds::SetMinMax(Vec3 min, Vec3 max) {
        extends = (max - min) * 0.5f;
        center = min + extends;
    }

    String Bounds::ToString() const {
        return StringUtils::Format("(Center: {}, Extends: {})", center.ToString(), extends.ToString());
    }

}
