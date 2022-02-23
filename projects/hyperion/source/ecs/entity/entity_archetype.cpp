//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/entity/entity_archetype.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  EntityArchetype::EntityArchetype(EntityGuid guid, EntityArchetypeComponentStorage storage) {
    m_guid = guid;
    m_storage = storage;
  }

}