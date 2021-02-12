#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/audio/audio_driver.hpp"

//-------------------- Forward Declarations --------------------
namespace FMOD {
    class System;
    class Sound;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

    class FModAudioDriver : public IAudioDriver {
    public:
        AudioBackend GetBackend() const override { return AudioBackend::FMod; }

        void Initialize() override;
        void Shutdown() override;

        void Load(const String &name, const String &path) override;
        void Play(const String &name) override;
    private:
        FMOD::System *m_system;

        Map<String, FMOD::Sound *> m_sounds;
    };

}