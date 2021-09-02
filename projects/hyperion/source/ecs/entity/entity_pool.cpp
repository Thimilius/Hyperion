//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/component/component_pool.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    ComponentPool::ComponentPool(uint64 max_entities, ComponentInfo component_info) : m_component_info(component_info) {
        m_entity_indices.Resize(max_entities, SPARSE_ELEMENT);
    }

    //--------------------------------------------------------------
    byte *ComponentPool::AddComponent(EntityId id) {
        uint32 packed_index = m_entity_indices[EntityUtilities::GetIndex(id)];
        if (packed_index == ComponentPool::SPARSE_ELEMENT) {
            m_entity_indices[EntityUtilities::GetIndex(id)] = static_cast<uint32>(m_entity_list.GetLength());
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
    bool ComponentPool::HasComponent(EntityId id) {
        uint32 packed_index = m_entity_indices[EntityUtilities::GetIndex(id)];
        return packed_index != ComponentPool::SPARSE_ELEMENT;
    }

    //--------------------------------------------------------------
    byte *ComponentPool::GetComponent(EntityId id) {
        uint32 packed_index = m_entity_indices[EntityUtilities::GetIndex(id)];
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
    bool ComponentPool::RemoveComponent(EntityId id) {
        uint32 packed_index = m_entity_indices[EntityUtilities::GetIndex(id)];
        if (packed_index != ComponentPool::SPARSE_ELEMENT) {
            m_entity_indices[EntityUtilities::GetIndex(id)] = ComponentPool::SPARSE_ELEMENT;

            EntityId packed_id = m_entity_list[packed_index];
            HYP_ASSERT(packed_id == id);

            EntityId repacked_id = m_entity_list.GetLast();
            m_entity_list.RemoveLast();
            if (packed_id != repacked_id) {
                m_entity_list[packed_index] = repacked_id;
                m_entity_indices[repacked_id] = packed_index;
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

}