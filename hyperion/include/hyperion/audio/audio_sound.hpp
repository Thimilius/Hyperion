//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

  class IAudioSound {
  public:
    virtual ~IAudioSound() = default;
  };

  class NullAudioSound final : public IAudioSound {
    
  };
  
}
  