//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <rttr/registration.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  using Type = rttr::type;
  using Variant = rttr::variant;
  using Instance = rttr::instance;
  
  class Reflection final {
  public:
    inline static Instance CreateInstanceFromType(void *data, Type type) {
      Instance i = data;
      (reinterpret_cast<rttr::detail::data_address_container *>(&i))->m_type = type;
      return i;
    }
  private:
    Reflection() = delete;
    ~Reflection() = delete;
  };
  
}
