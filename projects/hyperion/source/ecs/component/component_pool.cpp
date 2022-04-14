//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/component/component_pool.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  ComponentPool::ComponentPool(ComponentInfo component_info) : m_component_info(component_info) {
    FitIntoPool(0);
  }

  //--------------------------------------------------------------
  byte *ComponentPool::AddComponent(EntityId id) {
    EntityIndices &entity_indices = GetEntityIndices(id);
    uint32 sparse_index = GetSparseIndex(id);
    uint32 packed_index = entity_indices[sparse_index];
    if (packed_index == ComponentPool::SPARSE_ELEMENT) {
      entity_indices[sparse_index] = static_cast<uint32>(m_entity_list.GetLength());
      m_entity_list.Add(id);
      uint64 current_size = m_component_list.GetLength();
      m_component_list.Resize(current_size + m_component_info.element_size);

      byte *component_data = m_component_list.GetData() + current_size;
      return component_data;
    } else {
      return nullptr;
    }
  }

  //--------------------------------------------------------------
  bool8 ComponentPool::HasComponent(EntityId id) {
    EntityIndices &entity_indices = GetEntityIndices(id);
    uint32 sparse_index = GetSparseIndex(id);
    uint32 packed_index = entity_indices[sparse_index];
    return packed_index != ComponentPool::SPARSE_ELEMENT;
  }

  //--------------------------------------------------------------
  byte *ComponentPool::GetComponent(EntityId id) {
    EntityIndices &entity_indices = GetEntityIndices(id);
    uint32 sparse_index = GetSparseIndex(id);
    uint32 packed_index = entity_indices[sparse_index];
    if (packed_index == ComponentPool::SPARSE_ELEMENT) {
      return nullptr;
    } else {
      EntityId packed_id = m_entity_list[packed_index];
      HYP_ASSERT(packed_id == id);

      byte *component_data = m_component_list.GetData() + packed_index * m_component_info.element_size;
      return component_data;
    }
  }

  //--------------------------------------------------------------
  bool8 ComponentPool::RemoveComponent(EntityId id) {
    EntityIndices &entity_indices = GetEntityIndices(id);
    uint32 sparse_index = GetSparseIndex(id);
    uint32 packed_index = entity_indices[sparse_index];
    if (packed_index != ComponentPool::SPARSE_ELEMENT) {
      entity_indices[sparse_index] = ComponentPool::SPARSE_ELEMENT;

      EntityId packed_id = m_entity_list[packed_index];
      HYP_ASSERT(packed_id == id);

      EntityId repacked_id = m_entity_list.GetLast();
      m_entity_list.RemoveLast();
      if (packed_id != repacked_id) {
        m_entity_list[packed_index] = repacked_id;
        EntityIndices &repacked_entity_indices = GetEntityIndices(repacked_id);
        uint32 repacked_sparse_index = GetSparseIndex(repacked_id);
        repacked_entity_indices[repacked_sparse_index] = packed_index;
      }

      uint64 current_size = m_component_list.GetLength();
      byte *packed_component_data = m_component_list.GetData() + packed_index * m_component_info.element_size;
      byte *repacked_component_data = m_component_list.GetData() + (current_size - m_component_info.element_size);

      m_component_info.destructor(packed_component_data);
      if (packed_component_data != repacked_component_data) {
        std::memcpy(packed_component_data, repacked_component_data, m_component_info.element_size);
      }
      m_component_list.Resize(current_size - m_component_info.element_size);
      return true;
    } else {
      return false;
    }
  }

  //--------------------------------------------------------------
  void ComponentPool::FitIntoPool(EntityId id) {
    EntityIndex index = EntityUtilities::GetIndex(id);
    uint32 page_index = index / MAX_ENTITIES_PER_PAGE;

    if (page_index >= m_entity_index_pages.GetLength()) {
      m_entity_index_pages.Resize(m_entity_index_pages.GetLength() + 1);
      m_entity_index_pages[page_index].Resize(MAX_ENTITIES_PER_PAGE, SPARSE_ELEMENT);
    }
  }

  //--------------------------------------------------------------
  uint32 ComponentPool::GetSparseIndex(EntityId id) {
    return EntityUtilities::GetIndex(id) % MAX_ENTITIES_PER_PAGE;
  }

  //--------------------------------------------------------------
  Array<uint32> &ComponentPool::GetEntityIndices(EntityId id) {
    EntityIndex index = EntityUtilities::GetIndex(id);
    uint32 page_index = index / MAX_ENTITIES_PER_PAGE;
    return m_entity_index_pages[page_index];
  }

}
