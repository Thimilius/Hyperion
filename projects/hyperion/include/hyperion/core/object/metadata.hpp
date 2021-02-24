#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class Metadata {
        DontSerialize, // -> Bool - Default: false

        RequiresComponent0, // -> Type - Default: none
        RequiresComponent1, // -> Type - Default: none
    };

}