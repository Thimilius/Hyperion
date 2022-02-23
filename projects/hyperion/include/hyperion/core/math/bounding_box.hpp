//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/core/math/ray.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct BoundingBox {
    Vector3 min;
    Vector3 max;

    BoundingBox();
    BoundingBox(Vector3 min, Vector3 max);

    Vector3 GetSize() const;
    Vector3 GetCenter() const;

    bool8 Intersects(BoundingBox other) const;
    bool8 Intersects(Ray ray, float32 &hit_distance) const;
    bool8 Contains(Vector3 point) const;

    String ToString() const;

    static BoundingBox Transform(const Matrix4x4 &local_to_world, const BoundingBox &local);

    bool8 operator==(const BoundingBox &other) const;
    bool8 operator!=(const BoundingBox &other) const;
  };

}