//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/reflection.hpp"
#include "hyperion/core/string_utils.hpp"

#define HYP_OBJECT(TYPE)                                                           \
    public:                                                                        \
        inline virtual Type GetType() const override { return Type::get<TYPE>(); } \
    private:

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class Object {
  public:
    virtual ~Object() = default;
  public:
    inline virtual Type GetType() const { return Type::get<Object>(); }

    inline String GetName() const { return m_name; }
    inline void SetName(const String &name) { m_name = name; }

    virtual String ToString() const { return m_name; }
  private:
    String m_name;
  };

}
