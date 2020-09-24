#include "hyppch.hpp"

#include "hyperion/audio/audio_engine.hpp"

#include "hyperion/modules/fmod/fmod_audio_driver.hpp"

namespace Hyperion::Audio {

    void AudioEngine::Init(const AudioSettings &settings) {
        s_audio_backend = settings.backend;

        switch (settings.backend) {
            case AudioBackend::None:
                s_audio_driver = new DummyAudioDriver();
                break;
            case AudioBackend::FMod:
                s_audio_driver = new FModAudioDriver();
                break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return;
        }

        s_audio_driver->Init();
    }

}