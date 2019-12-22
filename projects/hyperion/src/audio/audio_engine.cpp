#include "hyppch.hpp"

#include "hyperion/audio/audio_engine.hpp"

#include "hyperion/driver/fmod/fmod_audio_driver.hpp"

namespace Hyperion::Audio {

    void AudioEngine::Init(AudioBackend backend) {
        switch (backend) {
            case Hyperion::Audio::AudioBackend::None:
                s_audio_driver.reset(new DummyAudioDriver());
                break;
            case Hyperion::Audio::AudioBackend::FMod:
                s_audio_driver.reset(new FModAudioDriver());
                break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return;
        }

        s_audio_backend = backend;
        s_audio_driver->Init();
    }

    void AudioEngine::Shutdown() {
        s_audio_driver->Shutdown();
    }

}