#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class Metadata {
        Serialize, // -> Bool - Default: true

        RequiresComponents, // -> Vector<Variant> (Varinat just stores instances of Type) - Default: none
    };

}