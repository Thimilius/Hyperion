//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//-------------------- Definition Namespace --------------------
#include "hyperion/assets/utilities/texture_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  uint32 TextureUtilities::CalculateMipmapCount(uint32 width, uint32 height) {
    return static_cast<uint32>(1 + Math::Floor(Math::Log2(Math::Max(static_cast<float32>(width), static_cast<float32>(height)))));
  }

}
