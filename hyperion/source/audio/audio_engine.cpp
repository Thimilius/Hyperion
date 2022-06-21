//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/audio/audio_engine.hpp"

//---------------------- Project Includes ----------------------
#ifdef HYP_AUDIO_SOLOUD
#include "hyperion/modules/soloud/soloud_audio_driver.hpp"
#endif

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

  //--------------------------------------------------------------
  void AudioEngine::Initialize() {
#ifdef HYP_AUDIO_SOLOUD
    s_audio_driver = new SoLoudAudioDriver();
#else
    s_audio_driver = new NullAudioDriver();
#endif

    s_audio_driver->Initialize();
  }

  //--------------------------------------------------------------
  void AudioEngine::Shutdown() {
    s_audio_driver->Shutdown();
    delete s_audio_driver;
  }

}
