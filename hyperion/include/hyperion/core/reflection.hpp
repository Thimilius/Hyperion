//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <rttr/registration.h>
#include <rttr/rttr_enable.h>

#define HYP_REFLECT(...) RTTR_ENABLE(__VA_ARGS__)
#define HYP_REFLECT_REGISTER RTTR_REGISTRATION

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  using Variant = rttr::variant;
  using Instance = rttr::instance;
  using Type = rttr::type;
  using Property = rttr::property;
  using Enumeration = rttr::enumeration;
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
      Instance instance = object;
      (reinterpret_cast<rttr::detail::data_address_container *>(&instance))->m_type = type;
      return instance;
    }

    inline static void *GetVariantData(Variant &variant) {
      return rttr::detail::unsafe_variant_cast<void *>(&variant);
    }
  private:
    Reflection() = delete;
    ~Reflection() = delete;
  };

  enum class PropertyMetadata {
    Serialize, // Value: bool8, Default: true
  };

  enum class TypeMetadata {
    HideInEditor, // Value: bool8, Default: false
    EditorName, // Value: String, Default: Name of type
    EditorRemovable, // Value: bool8, Default: true
  };
  
}
