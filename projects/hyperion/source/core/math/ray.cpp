//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/ray.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Ray::Ray(Vec3 origin, Vec3 direction) :
        origin(origin), direction(direction.Normalized()) { }

    //--------------------------------------------------------------
    Vec3 Ray::GetPoint(float32 t) const {
        return origin + direction * t;
    }

    //--------------------------------------------------------------
    String Ray::ToString() const {
        return StringUtils::Format("Origin: {}, Direction: {}", origin.ToString(), direction.ToString());
    }

}