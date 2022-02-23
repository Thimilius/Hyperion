//------------------------ Header Guard ------------------------
#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  enum class TypeAttribute {
    EnumFlags, // Value: bool8, Default: false
  };

  enum class PropertySpecialSerialize {
    Raw,
    EntityIdAsGuid,
    PointerAsAssetGuid
  };

  enum class PropertyAttribute {
    Serialize, // Value: bool8, Default: true
    SpecialSerialize // Value: PropertySpecialSerialize, Default: PropertySpecialSerialize::Raw
  };

}