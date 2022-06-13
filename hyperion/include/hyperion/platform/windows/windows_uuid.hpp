//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class WindowsUUID {
  public:
    inline bool IsValid() const { return m_data[0] != 0 && m_data[1] != 0; }
    
    String ToString() const;

    bool8 operator==(const WindowsUUID &other) const;
    bool8 operator!=(const WindowsUUID &other) const;
  public:
    static WindowsUUID Generate();
    static WindowsUUID Generate(const String &string);
  private:
    uint64 m_data[2] = { 0 };
  private:
    friend struct std::hash<Hyperion::WindowsUUID>;
  };

}
