//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include "hyperion/core/log.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  struct EditorLogEntry {
    LogLevel level;
    String message;
  };
  
  class EditorLogger final : public ILogger {
  public:
    inline void Log(LogType type, LogLevel level, const String &message) override {
      if (type == LogType::Client) {
        m_log_entries.Add({ level, message });
      }
    }

    inline bool8 ShouldClearOnPlay() const { return m_should_clear_on_play; }
    inline void SetShouldClearOnPlay(bool8 should_clear_on_play) { m_should_clear_on_play = should_clear_on_play; }
    
    const Array<EditorLogEntry> &GetLogEntries() const { return m_log_entries; }

    inline void Clear() { m_log_entries.Clear(); }
  private:
    Array<EditorLogEntry> m_log_entries;
    
    bool8 m_should_clear_on_play = true; 
  };

}