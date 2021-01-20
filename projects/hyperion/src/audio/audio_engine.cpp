#include "hyppch.hpp"

#include "hyperion/audio/audio_engine.hpp"

#ifdef HYP_AUDIO_FMOD
#include "hyperion/modules/fmod/fmod_audio_driver.hpp"
#endif

namespace Hyperion::Audio {

    void AudioEngine::Init() {
#ifdef HYP_AUDIO_FMOD
        s_audio_driver = new FModAudioDriver();
#else
        s_audio_driver = new DummyAudioDriver();
#endif

        s_audio_driver->Init();
    }

    void AudioEngine::Shutdown() {
        s_audio_driver->Shutdown();
        delete s_audio_driver;
    }

}