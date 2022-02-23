//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class WindowsGuid {
  public:
    String ToString() const;

    bool8 operator==(const WindowsGuid &other) const;
    bool8 operator!=(const WindowsGuid &other) const;
  public:
    static WindowsGuid Generate();
    static WindowsGuid Generate(const String &string);
  private:
    uint64 m_data[2] = { 0 };
  private:
    friend struct std::hash<Hyperion::WindowsGuid>;
  };

}