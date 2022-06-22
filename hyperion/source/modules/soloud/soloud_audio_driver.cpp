//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/soloud/soloud_audio_driver.hpp"

//------------------------- Namespaces -------------------------
using namespace SoLoud;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

  //--------------------------------------------------------------
  void SoLoudAudioDriver::Initialize() {
    m_soloud = new Soloud();
    m_soloud->init();

    HYP_LOG_INFO("Audio", "Initialized SoLoud audio driver!");
  }

  //--------------------------------------------------------------
  void SoLoudAudioDriver::Shutdown() {
    for (SoLoudAudioSound *sound : m_sounds) {
      delete sound;
    }
    
    m_soloud->deinit();
    delete m_soloud;
  }

  //--------------------------------------------------------------
  IAudioSound *SoLoudAudioDriver::LoadSound(const String &path) {
    SoLoudAudioSound *sound = new SoLoudAudioSound();

    result load_result = sound->m_sample.load(path.c_str());
    if (load_result != SOLOUD_ERRORS::SO_NO_ERROR) {
      delete sound;
      return nullptr;
    }

    m_sounds.Add(sound);
    return sound;
  }

  //--------------------------------------------------------------
  void SoLoudAudioDriver::PlaySound(IAudioSound *sound) {
    HYP_ASSERT(sound);
    
    m_soloud->play(static_cast<SoLoudAudioSound *>(sound)->m_sample);
  }

}
