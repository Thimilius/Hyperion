//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/threading/windows_thread.hpp"

//---------------------- Library Includes ----------------------
#include <Windows.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Threading {

  //--------------------------------------------------------------
  ThreadId WindowsThread::GetId() {
    return GetThreadId(m_thread.native_handle());
  }

  //--------------------------------------------------------------
  void WindowsThread::Start(const ThreadStartFunction &start_function) {
    m_thread = std::thread(start_function);
  }

  //--------------------------------------------------------------
  void WindowsThread::Start(const ParameterizedThreadStartFunction &parameterized_start_function, void *parameter) {
    m_thread = std::thread(parameterized_start_function, parameter);
  }

  //--------------------------------------------------------------
  void WindowsThread::Join() {
    m_thread.join();
  }

  //--------------------------------------------------------------
  void WindowsThread::SetName(const String &name) {
    WideString name_wide = StringUtils::Utf8ToUtf16(name);
    SetThreadDescription(m_thread.native_handle(), name_wide.data());
  }

  //--------------------------------------------------------------
  uint32 WindowsThread::GetSupportedThreadCount() {
    return std::thread::hardware_concurrency();
  }

  //--------------------------------------------------------------
  ThreadId WindowsThread::GetCurrentThreadId() {
    return ::GetCurrentThreadId();
  }

  //--------------------------------------------------------------
  void WindowsThread::Sleep(uint32 milliseconds) {
    std::chrono::milliseconds duration(milliseconds);
    std::this_thread::sleep_for(duration);
  }

}
