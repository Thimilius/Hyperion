#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class Metadata {
        Flags, // Marks an enum as multiple flags -> Bool - Default: false

        DontSerialize, // Marks a property to not be serialized -> Bool - Default: false

        RequiresComponent0, // Marks a component to require another component -> Type - Default: none
        RequiresComponent1, // Marks a component to require another component -> Type - Default: none
    };

}