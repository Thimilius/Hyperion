//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <rttr/registration.h>

#define HYP_REFLECT_REGISTER RTTR_REGISTRATION

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  using Variant = rttr::variant;
  using Instance = rttr::instance;
  using Type = rttr::type;
  using Property = rttr::property;
  using Registration = rttr::registration;

  template<typename Enum_Type>
  RTTR_INLINE auto Value(rttr::string_view name, Enum_Type value)
  {
    return rttr::value(name, value);
  }

  RTTR_INLINE auto Metadata(Variant key, Variant value)
  {
    return rttr::metadata(key, value);
  }
  
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
