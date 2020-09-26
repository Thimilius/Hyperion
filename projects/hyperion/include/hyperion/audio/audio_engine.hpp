#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/audio/audio_driver.hpp"

namespace Hyperion {
    class Engine;
}

namespace Hyperion::Audio {

    class AudioEngine final {
    public:
        inline static AudioBackend GetBackend() { return s_audio_backend; }

        inline static void Load(const String &name, const String &path) { s_audio_driver->Load(name, path); }
        inline static void Play(const String &name) { s_audio_driver->Play(name); }
    private:
        AudioEngine() = delete;
        ~AudioEngine() = delete;

        static void Init(const AudioSettings &settings);
        static void Shutdown();
    private:
        inline static AudioBackend s_audio_backend;

        inline static IAudioDriver *s_audio_driver;
    private:
        friend class Hyperion::Engine;
    };

}