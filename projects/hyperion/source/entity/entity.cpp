//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/entity.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    EntityPool::EntityPool(uint64 max_entities, ComponentId component_id, uint64 component_element_size, ComponentDestructorFunction component_destructor) {
        m_entity_indices.Resize(max_entities, SPARSE_ELEMENT);

        m_component_id = component_id;
        m_component_element_size = component_element_size;
        m_component_destructor = component_destructor;
    }

    //--------------------------------------------------------------
    byte *EntityPool::AddComponent(EntityId id) {
        uint32 packed_index = m_entity_indices[Entity::GetIndex(id)];
        if (packed_index == EntityPool::SPARSE_ELEMENT) {
            m_entity_indices[Entity::GetIndex(id)] = static_cast<uint32>(m_entity_list.GetLength());
            m_entity_list.Add(id);
            uint64 current_size = m_component_list.GetLength();
            m_component_list.Resize(current_size + m_component_element_size);

            byte *component_data = m_component_list.GetData() + current_size;
            return component_data;

        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    byte *EntityPool::GetComponent(EntityId id) {
        uint32 packed_index = m_entity_indices[Entity::GetIndex(id)];
        if (packed_index == EntityPool::SPARSE_ELEMENT) {
            return nullptr;
        } else {
            EntityId packed_id = m_entity_list[packed_index];
            HYP_ASSERT(packed_id == id);

            byte *component_data = m_component_list.GetData() + packed_index * m_component_element_size;
            return component_data;
        }
    }

    //--------------------------------------------------------------
    bool EntityPool::RemoveComponent(EntityId id) {
        uint32 packed_index = m_entity_indices[Entity::GetIndex(id)];
        if (packed_index != EntityPool::SPARSE_ELEMENT) {
            m_entity_indices[Entity::GetIndex(id)] = EntityPool::SPARSE_ELEMENT;

            EntityId packed_id = m_entity_list[packed_index];
            HYP_ASSERT(packed_id == id);

            EntityId repacked_id = m_entity_list.GetLast();
            m_entity_list.RemoveLast();
            if (packed_id != repacked_id) {
                m_entity_list[packed_index] = repacked_id;
                m_entity_indices[repacked_id] = packed_index;
            }

            uint64 current_size = m_component_list.GetLength();
            byte *packed_component_data = m_component_list.GetData() + packed_index * m_component_element_size;
            byte *repacked_component_data = m_component_list.GetData() + (current_size - m_component_element_size);

            m_component_destructor(packed_component_data);
            if (packed_component_data != repacked_component_data) {
                std::memcpy(packed_component_data, repacked_component_data, m_component_element_size);
            }
            m_component_list.Resize(current_size - m_component_element_size);
            return true;
        } else {
            return false;
        }
    }

}