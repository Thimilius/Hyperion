//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/bounding_box.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Mesh;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class MeshUtilities final {
  public:
    static BoundingBox CalculateBounds(const Array<Vector3> &positions);
  private:
    MeshUtilities() = delete;
    ~MeshUtilities() = delete;
  };

}