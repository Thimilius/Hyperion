//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/meta/meta.hpp"
#include "hyperion/core/meta/meta_factory.hpp"

#define HYP_OBJECT(TYPE)                                                                           \
    public:                                                                                        \
        inline virtual MetaType GetType() const override { return MetaRegistry::Resolve<TYPE>(); } \
    private:                                                                                       \

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class Object {
  public:
    virtual ~Object() = default;
  public:
    inline virtual MetaType GetType() const { return MetaRegistry::Resolve<Object>(); }

    inline String GetName() const { return m_name; }
    inline void SetName(const String &name) { m_name = name; }

    virtual String ToString() const { return m_name; }
  private:
    String m_name;
  };

}