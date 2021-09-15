#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class World;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class WorldHierarchy final {
    public:
        const Array<EntityId> &GetRoots() const { return m_roots; }

        void SetParent(EntityId entity, EntityId parent);
    private:
        World *m_world;
        Array<EntityId> m_roots;
    private:
        friend class Hyperion::World;
    };

}