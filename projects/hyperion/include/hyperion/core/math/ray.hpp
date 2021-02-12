#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vec3.hpp"


//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct Ray {
        Vec3 origin;
        Vec3 direction;

        Ray(Vec3 origin, Vec3 direction);

        Vec3 GetPoint(float32 t) const;

        String ToString() const;
    };

}