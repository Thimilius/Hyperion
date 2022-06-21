//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <soloud/soloud.h>

//---------------------- Project Includes ----------------------
#include "hyperion/audio/audio_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

  class SoLoudAudioDriver final : public IAudioDriver {
  public:
    inline AudioBackend GetBackend() const override { return AudioBackend::SoLoud; }

    void Initialize() override;
    void Shutdown() override;
  private:
    SoLoud::Soloud *m_soloud = nullptr;
  };
  
}
