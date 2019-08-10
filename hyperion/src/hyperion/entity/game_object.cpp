#include "hyppch.hpp"

#include "hyperion/entity/game_object.hpp"

namespace Hyperion {

    CGameObject::CGameObject(const CString &name) : m_name(name) {
        m_transform = new CTransformComponent();
        AddComponentInternal<CTransformComponent>(m_transform);
    }

    CGameObject::~CGameObject() {
        for (auto component : m_components) {
            delete component.second;
        }
    }

}