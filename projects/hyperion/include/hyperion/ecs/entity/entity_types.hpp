//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/guid.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using EntityId = uint64;
    using EntityIndex = uint32;
    using EntityVersion = uint32;
    using EntityGuid = Guid;

    struct EntityDescription {
        EntityId id;
        EntityGuid guid;
    };

    namespace Entity {
        inline static constexpr EntityId EMPTY = 0xFFFFFFFF;
    }

    enum class EntityPrimitive {
        Empty,
        Base,

        Camera,

        DirectionalLight,
        PointLight,
        SpotLight,

        Quad,
        Plane,
        Cube,
        Sphere,

        Sprite
    };

}