//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/audio/audio_backend.hpp"
#include "hyperion/audio/audio_sound.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

  class IAudioDriver {
  public:
    virtual ~IAudioDriver() = default;
  public:
    virtual AudioBackend GetBackend() const = 0;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual IAudioSound *LoadSound(const String &path) = 0;
    virtual void PlaySound(IAudioSound *sound) = 0;
  };

  class NullAudioDriver final : public IAudioDriver {
  public:
    AudioBackend GetBackend() const override { return AudioBackend::None; }

    void Initialize() override {
      HYP_LOG_INFO("Audio", "Initialized Null audio driver!");
    }

    void Shutdown() override { }

    IAudioSound *LoadSound(const String &path) override { return new NullAudioSound(); }
    void PlaySound(IAudioSound *sound) override { }
  };

}
