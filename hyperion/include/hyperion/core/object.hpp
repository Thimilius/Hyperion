//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/reflection.hpp"
#include "hyperion/core/string_utils.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class Object {
    HYP_REFLECT()
  public:
    virtual ~Object() = default;
  public:
    inline String GetName() const { return m_name; }
    inline void SetName(const String &name) { m_name = name; }

    virtual String ToString() const { return m_name; }
  private:
    String m_name;
  };

}
