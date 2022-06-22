//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <soloud/soloud.h>

//---------------------- Project Includes ----------------------
#include "hyperion/audio/audio_driver.hpp"
#include "hyperion/modules/soloud/soloud_audio_sound.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

  class SoLoudAudioDriver final : public IAudioDriver {
  public:
    inline AudioBackend GetBackend() const override { return AudioBackend::SoLoud; }

    void Initialize() override;
    void Shutdown() override;

    IAudioSound *LoadSound(const String &path) override;
    void PlaySound(IAudioSound *sound) override;
  private:
    SoLoud::Soloud *m_soloud = nullptr;

    Array<SoLoudAudioSound *> m_sounds;
  };
  
}
