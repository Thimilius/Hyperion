//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <rttr/registration.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  using Variant = rttr::variant;
  using Instance = rttr::instance;
  using Type = rttr::type;
  using Property = rttr::property; 
  
  class Reflection final {
  public:
    inline static Instance CreateInstanceFromRaw(Type type, void *object) {
      Instance i = object;
      (reinterpret_cast<rttr::detail::data_address_container *>(&i))->m_type = type;
      return i;
    }
  private:
    Reflection() = delete;
    ~Reflection() = delete;
  };

  enum class PropertyAttribute {
    Serialize, // Value: bool8, Default: true
  };
  
}
