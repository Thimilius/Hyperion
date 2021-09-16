#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vector3.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct Ray {
        Vector3 origin;
        Vector3 direction;

        Ray();
        Ray(Vector3 origin, Vector3 direction);

        Vector3 GetPoint(float32 t) const;

        String ToString() const;
    };

}