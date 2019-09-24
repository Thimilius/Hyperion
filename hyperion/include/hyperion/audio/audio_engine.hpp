#pragma once

#include "hyperion/common.hpp"
#undef PlaySound

namespace FMOD {
    class System;
    class Sound;
}

namespace Hyperion::Audio {

    class CAudioEngine {
    private:
        inline static FMOD::System *m_system;

        inline static TMap<TString, FMOD::Sound*> m_sounds;
    public:
        static void LoadSound(const TString &name, const TString &path);
        static void PlaySound(const TString &name);
    private:
        CAudioEngine() = delete;
        ~CAudioEngine() = delete;

        static void Init();
        static void Shutdown();

        friend class CEngine;
    };

}