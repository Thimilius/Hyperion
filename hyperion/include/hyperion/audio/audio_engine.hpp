//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/audio/audio_driver.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

  class AudioEngine final {
  public:
    static IAudioSound *LoadSound(const String &path);
    static void PlaySound(IAudioSound *sound);
  private:
    AudioEngine() = delete;
    ~AudioEngine() = delete;
  private:
    static void Initialize();
    static void Shutdown();
  private:
    inline static IAudioDriver *s_audio_driver;
  private:
    friend class Hyperion::Engine;
  };

}
