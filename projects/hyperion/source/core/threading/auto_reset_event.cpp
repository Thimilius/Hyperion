//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/threading/auto_reset_event.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Threading {

  //--------------------------------------------------------------
  AutoResetEvent::AutoResetEvent(bool8 start_value) {
    m_signaled = start_value;
  }

  //--------------------------------------------------------------
  void AutoResetEvent::Notify() {
    std::unique_lock<std::mutex> lock(m_mutex);

    m_signaled = true;
    m_condition_variable.notify_one();
  }

  //--------------------------------------------------------------
  void AutoResetEvent::Wait() {
    std::unique_lock<std::mutex> lock(m_mutex);

    m_condition_variable.wait(lock, [&]() {
      bool8 signaled = m_signaled;
      return signaled;
    });
    m_signaled = false;
  }

  //--------------------------------------------------------------
  bool8 AutoResetEvent::WaitUnblocked() {
    if (m_signaled) {
      m_signaled = false;
      return true;
    } else {
      return false;
    }
  }

}