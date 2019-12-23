#pragma once

#include "hyperion/common.hpp"
#include "hyperion/audio/audio_backend.hpp"
#include "hyperion/audio/audio_driver.hpp"

namespace Hyperion {
    class Engine;
}

namespace Hyperion::Audio {

    class AudioEngine {
    private:
        inline static AudioBackend s_audio_backend;

        inline static Scope<AudioDriver> s_audio_driver;
    public:
        inline static void Load(const String &name, const String &path) {
            s_audio_driver->Load(name, path);
        }

        inline static void Play(const String &name) {
            s_audio_driver->Play(name);
        }

        inline static AudioBackend GetBackend() { return s_audio_backend; }
    private:
        AudioEngine() = delete;
        ~AudioEngine() = delete;

        static void Init(AudioBackend backend);
        static void Shutdown();

        friend class Hyperion::Engine;
    };

}