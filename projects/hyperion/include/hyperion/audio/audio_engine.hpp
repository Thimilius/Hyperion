#pragma once

#include "hyperion/common.hpp"
#include "hyperion/audio/audio_driver.hpp"

namespace Hyperion {
    class Engine;
}

namespace Hyperion::Audio {

    enum class AudioBackend {
        None,
        FMod
    };

    class AudioEngine {
    private:
        inline static AudioBackend s_audio_backend;

        inline static Scope<AudioDriver> s_audio_driver;
    public:
        static inline void Load(const String &name, const String &path) {
            s_audio_driver->Load(name, path);
        }

        static inline void Play(const String &name) {
            s_audio_driver->Play(name);
        }

        static inline AudioBackend GetBackend() { return s_audio_backend; }
    private:
        AudioEngine() = delete;
        ~AudioEngine() = delete;

        static void Init(AudioBackend backend);
        static void Shutdown();

        friend class Hyperion::Engine;
    };

}