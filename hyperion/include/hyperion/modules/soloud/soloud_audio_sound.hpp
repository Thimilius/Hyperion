//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <soloud/soloud_wav.h>

//---------------------- Project Includes ----------------------
#include "hyperion/audio/audio_sound.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Audio {
  class SoLoudAudioDriver;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

  class SoLoudAudioSound final : public IAudioSound {
  private:
    SoLoud::Wav m_sample;
  private:
    friend class Hyperion::Audio::SoLoudAudioDriver;
  };
  
}
  