#pragma once

#include "hyperion/common.hpp"

namespace FMOD {
    class System;
    class Sound;
}

namespace Hyperion::Audio {

    class AudioEngine {
    private:
        inline static FMOD::System *m_system;

        inline static Map<String, FMOD::Sound*> m_sounds;
    public:
        static void LoadSound(const String &name, const String &path);
        static void PlaySound(const String &name);
    private:
        AudioEngine() = delete;
        ~AudioEngine() = delete;

        static void Init();
        static void Shutdown();

        friend class Hyperion::Engine;
    };

}