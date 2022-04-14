//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/random.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  float32 Random::Get() {
    return static_cast<float32>(rand()) / static_cast<float32>(RAND_MAX);
  }

}
