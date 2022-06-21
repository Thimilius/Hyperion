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
    m_soloud->deinit();
    delete m_soloud;
  }
  
}
