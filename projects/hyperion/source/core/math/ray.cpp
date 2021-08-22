//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/ray.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Ray::Ray(Vector3 origin, Vector3 direction) :
        origin(origin), direction(direction.Normalized()) { }

    //--------------------------------------------------------------
    Vector3 Ray::GetPoint(float32 t) const {
        return origin + direction * t;
    }

    //--------------------------------------------------------------
    String Ray::ToString() const {
        return StringUtils::Format("Origin: {}, Direction: {}", origin.ToString(), direction.ToString());
    }

}