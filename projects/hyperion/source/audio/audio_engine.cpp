//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/audio/audio_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

    //--------------------------------------------------------------
    void AudioEngine::Initialize() {
        s_audio_driver = new NullAudioDriver();

        s_audio_driver->Initialize();
    }

    //--------------------------------------------------------------
    void AudioEngine::Shutdown() {
        s_audio_driver->Shutdown();
        delete s_audio_driver;
    }

}