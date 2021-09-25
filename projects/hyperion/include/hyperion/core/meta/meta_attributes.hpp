#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class PropertySpecialSerialize {
        Raw,
        EntityIdAsGuid,
    };

    enum class PropertyAttribute {
        Serialize, // Value: bool8
        SpecialSerialize // Value: PropertySpecialSerialize
    };

}