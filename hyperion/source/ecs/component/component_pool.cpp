//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/component/component_pool.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  ComponentPool::ComponentPool(ComponentInfo component_info) : m_component_info(component_info) {
    FitIntoPool(0);
  }

  //--------------------------------------------------------------
  ComponentPool::ComponentPool(const ComponentPool &other) {
    Copy(other);
  }

  //--------------------------------------------------------------
  ComponentPool::~ComponentPool() {
    // We need to call the actual destructor for all our components.
    byte *data = m_component_data.GetData();
    for (uint64 i = 0; i < GetEntityCount(); i++) {
      const byte *instance = data + i * m_component_info.element_size;
      m_component_info.destructor(instance);
    }
  }

  //--------------------------------------------------------------
  ComponentPool &ComponentPool::operator=(const ComponentPool &other) {
    if (this == &other) {
      return *this;
    }
    
    Copy(other);
    return *this;    
  }

  //--------------------------------------------------------------
  void *ComponentPool::AddComponent(EntityId id) {
    EntityIndices &entity_indices = GetEntityIndices(id);
    uint32 sparse_index = GetSparseIndex(id);
    uint32 packed_index = entity_indices[sparse_index];
    if (packed_index == SPARSE_ELEMENT) {
      entity_indices[sparse_index] = static_cast<uint32>(m_entity_list.GetLength());
      m_entity_list.Add(id);
      uint64 current_size = m_component_data.GetLength();
      m_component_data.Resize(current_size + m_component_info.element_size);

      byte *component_data = m_component_data.GetData() + current_size;
      // We need to call the placement-new constructor to properly construct the type in place.
      void *component = m_component_info.constructor(component_data);
      return component;
    } else {
      return nullptr;
    }
  }

  //--------------------------------------------------------------
  bool8 ComponentPool::HasComponent(EntityId id) {
    EntityIndices &entity_indices = GetEntityIndices(id);
    uint32 sparse_index = GetSparseIndex(id);
    uint32 packed_index = entity_indices[sparse_index];
    return packed_index != SPARSE_ELEMENT;
  }

  //--------------------------------------------------------------
  byte *ComponentPool::GetComponent(EntityId id) {
    EntityIndices &entity_indices = GetEntityIndices(id);
    uint32 sparse_index = GetSparseIndex(id);
    uint32 packed_index = entity_indices[sparse_index];
    if (packed_index == SPARSE_ELEMENT) {
      return nullptr;
    } else {
      EntityId packed_id = m_entity_list[packed_index];
      HYP_ASSERT(packed_id == id);

      byte *component_data = m_component_data.GetData() + packed_index * m_component_info.element_size;
      return component_data;
    }
  }

  //--------------------------------------------------------------
  bool8 ComponentPool::RemoveComponent(EntityId id) {
    EntityIndices &entity_indices = GetEntityIndices(id);
    uint32 sparse_index = GetSparseIndex(id);
    uint32 packed_index = entity_indices[sparse_index];
    if (packed_index != SPARSE_ELEMENT) {
      entity_indices[sparse_index] = SPARSE_ELEMENT;

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

      uint64 current_size = m_component_data.GetLength();
      byte *packed_component_data = m_component_data.GetData() + packed_index * m_component_info.element_size;
      byte *repacked_component_data = m_component_data.GetData() + (current_size - m_component_info.element_size);

      void *destination_instance = packed_component_data;
      void *source_instance = repacked_component_data;

      // If we are the last component in the packed array then we need to call our destructor.
      // In every other case we call the move assignment operator which clears the memory of our removed component
      // and properly moves over the data from the component we are repacking. 
      if (packed_component_data == repacked_component_data) {
        m_component_info.destructor(source_instance);
      } else {
        m_component_info.move_assignment_operator(destination_instance, source_instance);
      }
      
      m_component_data.Resize(current_size - m_component_info.element_size);
      return true;
    } else {
      return false;
    }
  }

  //--------------------------------------------------------------
  void ComponentPool::FitIntoPool(EntityId id) {
    EntityIndex index = EntityId::GetIndex(id);
    uint32 page_index = index / MAX_ENTITIES_PER_PAGE;

    if (page_index >= m_entity_index_pages.GetLength()) {
      m_entity_index_pages.Resize(m_entity_index_pages.GetLength() + 1);
      m_entity_index_pages[page_index].Resize(MAX_ENTITIES_PER_PAGE, SPARSE_ELEMENT);
    }
  }

  //--------------------------------------------------------------
  uint32 ComponentPool::GetSparseIndex(EntityId id) {
    return EntityId::GetIndex(id) % MAX_ENTITIES_PER_PAGE;
  }

  //--------------------------------------------------------------
  Array<uint32> &ComponentPool::GetEntityIndices(EntityId id) {
    EntityIndex index = EntityId::GetIndex(id);
    uint32 page_index = index / MAX_ENTITIES_PER_PAGE;
    return m_entity_index_pages[page_index];
  }

  //--------------------------------------------------------------
  void ComponentPool::Copy(const ComponentPool &other) {
    m_component_info = other.m_component_info;
    m_entity_index_pages = other.m_entity_index_pages;
    m_entity_list = other.m_entity_list;
    
    // When copying a component pool we have to remember that we can not simply copy the component data.
    // We properly need to invoke their copy constructor for proper memory management.
    m_component_data.Resize(other.m_component_data.GetLength());
    byte *destination_data = m_component_data.GetData();
    const byte *source_data = other.m_component_data.GetData();
    for (uint64 i = 0; i < other.GetEntityCount(); i++) {
      byte *address = destination_data + i * m_component_info.element_size;
      const byte *instance = source_data + i * m_component_info.element_size;
      m_component_info.copy_constructor(address, instance);
    }
  }

}
