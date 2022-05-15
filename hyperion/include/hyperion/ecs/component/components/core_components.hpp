//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_archetype.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct NameComponent {
    String name;
  };

  struct TagComponent {
    String tag;
  };

  struct DisabledComponent { };

  struct StaticComponent { };

  // NOTE: The archetype component deviates quite heavily from regular components.
  // For example: We are fine with storing dynamic data here.
  struct ArchetypeComponent {
    EntityArchetype *archetype = nullptr;

    EntityArchetypeOverwrites overwrites;
    EntityArchetypeRemovedComponents removed_components;
  };

}
