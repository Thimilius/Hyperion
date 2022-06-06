//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/guid.hpp"
#include "hyperion/ecs/component/component_registry.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  using EntityIndices = Array<uint32>;
  using EntityIndexPages = Array<EntityIndices>;
  using EntityList = Array<EntityId>;
  using ComponentData = Array<byte>;

  class ComponentPool final {
  public:
    ComponentPool(ComponentInfo component_info);
    ComponentPool(const ComponentPool &other);
    ~ComponentPool();
    ComponentPool &operator=(const ComponentPool &other);
  public:
    inline uint64 GetEntityCount() const { return m_entity_list.GetLength(); }
    inline EntityId GetEntity(EntityIndex index) const { return m_entity_list[index]; }
    inline const ComponentInfo &GetComponentInfo() const { return m_component_info; }

    void *AddComponent(EntityId id);
    bool8 HasComponent(EntityId id);
    byte *GetComponent(EntityId id);
    bool8 RemoveComponent(EntityId id);

    void FitIntoPool(EntityId id);
  private:
    uint32 GetSparseIndex(EntityId id);
    Array<uint32> &GetEntityIndices(EntityId id);

    void Copy(const ComponentPool &other);
  public:
    inline static constexpr uint32 MAX_ENTITIES_PER_PAGE = 2048;
  private:
    ComponentInfo m_component_info = { };
    
    EntityIndexPages m_entity_index_pages;
    EntityList m_entity_list;
    ComponentData m_component_data;
  private:
    inline static const uint32 SPARSE_ELEMENT = EntityId::GetIndex(EntityId::EMPTY);
  };

}
