//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/entity/entity_archetype.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  EntityArchetype::EntityArchetype(EntityUUID uuid, EntityArchetypeComponentStorage storage) {
    m_uuid = uuid;
    m_storage = std::move(storage);
  }

}
