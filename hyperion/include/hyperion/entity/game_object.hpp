#pragma once

#include "hyperion/common.hpp"

#include "hyperion/entity/components/component.hpp"


namespace Hyperion {

    class CTransformComponent;

    class CGameObject {
    private:
        TString m_name;

        TMap<SComponentType*, CComponent*> m_components;

        CTransformComponent *m_transform;
    public:
        CGameObject() : CGameObject("New GameObject") {}
        CGameObject(const TString &name);
        ~CGameObject();

        inline const TString &GetName() const { return m_name; }
        inline void SetName(const TString &name) { m_name = name; }

        inline CTransformComponent *GetTransform() { return m_transform; }

        template<typename T>
        void AddComponent() {
            AddComponentInternal(new T());
        }

        template<typename T>
        T *GetComponent() {
            return GetComponentInternal<T>();
        }
    private:
        template<typename T>
        void AddComponentInternal(CComponent *component) {
            component->m_game_object = this;
            m_components[T::GetStaticType()] = component;
        }

        template<typename T>
        T *GetComponentInternal() const {
            SComponentType *type = T::GetStaticType();
            auto it = m_components.find(type);
            if (it == m_components.end())
                return nullptr;
            return (T *)it->second;
        }
    };

}