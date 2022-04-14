//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component.hpp"
#include "hyperion/ecs/entity/entity_archetype.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct NameComponent : public IComponent {
    String name;
  };

  struct TagComponent : public IComponent {
    String tag;
  };

  struct DisabledComponent : public IComponent { };

  struct StaticComponent : public IComponent { };

  // NOTE: The archetype component deviates quite heavily from regular components.
  // For example: We are fine with storing dynamic data here.
  struct ArchetypeComponent : public IComponent {
    EntityArchetype *archetype;

    EntityArchetypeOverwrites overwrites;
    EntityArchetypeRemovedComponents removed_components;
  };

}
