//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_types.hpp"
#include "hyperion/ecs/component/component_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    // ────────────────────────────────────────────────────────────────────────────────────
    // INTRODUCTION:
    // This is the documentation of how entity archetypes (prefabs) work.
    // Here are some goals the system tries to solve:
    //     - Prefabs should not disrupt the ECS (as in looping over components)
    //     - There should not be an unnecessary waste of memory
    // 
    // ENTITY ARCHETYPE:
    // The EntityArchetype class stores a single entity (meaning all its components) as an archetype.
    // This decouples the archetypes from the regular ECS world storage.
    // 
    // The actual archetype contains:
    //     - What types of components it has
    //     - The actual data for each component
    // 
    // Using the world, it is possible to instantiate an archetype which places a proper entity inside the world.
    // This creates an actual entity in the regular ECS storage with all relevant components and their data.
    // This is different from an entity which is referencing an archetype.
    // Referencing should only ever be done "outside the runtime", meaning "in the editor".
    // 
    // ARCHETYPE COMPONENT:
    // The ArchetypeComponent is the special component which ties an entity to its corresponding archetype.
    // This is simply a regular pointer to the archetype instance.
    // It can be used to query every related component data needed for the entity.
    // This component is however only used inside "the editor".
    // That means a world should only have entities with an ArchetypeComponent in the "editor state".
    // It is pretty much useless at runtime and is therefore always removed beforehand.
    // 
    // At its simplest form an entity which references an archetype simply consists of an ArchetypeComponent.
    // With this it's possible to display every inherited component and their attributes.
    // In this form it is however not possible to overwrite/change anything on the entity which deviates from the archetype.
    // Deviating could include: 
    //     - Overwriting the attributes of a component
    //     - Adding a new component
    //     - Removing an existing component
    // The data for overwriting attributes and removing components is direcly stored inside the ArchetypeComponent.
    // Adding new components is special as these components are just regularly added to the entity.
    // ────────────────────────────────────────────────────────────────────────────────────

    struct EntityArchetypeOverwrite {
        
    };

    using EntityArchetypeRemovedComponents = Array<ComponentId>;
    using EntityArchetypeOverwrites = Map<ComponentId, Array<EntityArchetypeOverwrite>>;
    using EntityArchetypeComponentStorage = Map<ComponentId, Array<byte>>;

    class EntityArchetype final {
    public:
        EntityArchetype(EntityGuid guid, EntityArchetypeComponentStorage storage);
    public:
        inline EntityGuid GetGuid() const { return m_guid; }
        inline const EntityArchetypeComponentStorage &GetStorage() const { return m_storage; }
    private:
        EntityGuid m_guid;
        EntityArchetypeComponentStorage m_storage;
    };

}