#include "hyppch.hpp"

#include "hyperion/entity/game_object.hpp"

#include "hyperion/entity/components/transform_component.hpp"

namespace Hyperion::Entity {

    CGameObject::CGameObject(const TString &name) : m_name(name) {
        m_transform = new CTransformComponent();
        AddComponentInternal<CTransformComponent>(m_transform);
    }

    CGameObject::~CGameObject() {
        for (auto component : m_components) {
            delete component.second;
        }
    }

}