//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/utilities/mesh_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  BoundingBox MeshUtilities::CalculateBounds(const Array<Vector3> &positions) {
    Vector3 min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
    Vector3 max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (uint32 i = 0; i < positions.GetLength(); i++) {
      Vector3 position = positions[i];

      min.x = Math::Min(min.x, position.x);
      min.y = Math::Min(min.y, position.y);
      min.z = Math::Min(min.z, position.z);
      max.x = Math::Max(max.x, position.x);
      max.y = Math::Max(max.y, position.y);
      max.z = Math::Max(max.z, position.z);
    }

    return BoundingBox(min, max);
  }

}
