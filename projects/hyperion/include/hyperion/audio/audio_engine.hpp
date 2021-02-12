#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/audio/audio_driver.hpp"
#include "hyperion/core/app/application_settings.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

    class AudioEngine final {
    public:
        inline static void Load(const String &name, const String &path) { s_audio_driver->Load(name, path); }
        inline static void Play(const String &name) { s_audio_driver->Play(name); }
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