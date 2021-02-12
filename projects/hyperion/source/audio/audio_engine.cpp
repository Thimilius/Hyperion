//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/audio/audio_engine.hpp"

//---------------------- Project Includes ----------------------
#ifdef HYP_AUDIO_FMOD
#include "hyperion/modules/fmod/fmod_audio_driver.hpp"
#endif

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

    //--------------------------------------------------------------
    void AudioEngine::Initialize() {
#ifdef HYP_AUDIO_FMOD
        s_audio_driver = new FModAudioDriver();
#else
        s_audio_driver = new DummyAudioDriver();
#endif

        s_audio_driver->Initialize();
    }

    //--------------------------------------------------------------
    void AudioEngine::Shutdown() {
        s_audio_driver->Shutdown();
        delete s_audio_driver;
    }

}