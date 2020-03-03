#pragma once

#include "hyperion/entity/components/behaviour.hpp"
#include "hyperion/rendering/material.hpp"

namespace Hyperion {

    class Renderer : public Behaviour {
        HYP_OBJECT(Renderer, Behaviour);
    private:
        Ref<Rendering::Material> m_shared_material;
        mutable Ref<Rendering::Material> m_material;
        mutable bool m_using_shared_material = true;
    public:
        Renderer() = default;
        Renderer(const String &name) : Behaviour(name) { }
        virtual ~Renderer() = default;

        inline Ref<Rendering::Material> GetSharedMaterial() const { return m_shared_material; }
        inline void SetSharedMaterial(const Ref<Rendering::Material> &shared_material) {
            m_shared_material = shared_material;
            m_using_shared_material = true;
        }

        inline Ref<Rendering::Material> GetMaterial() const {
            if (m_using_shared_material) {
                m_material = m_shared_material->Copy();
                m_using_shared_material = false;
            }
            return m_material;
        }
        inline void SetMaterial(const Ref<Rendering::Material> &material) {
            m_material = material;
            m_shared_material = material;
            m_using_shared_material = false;
        }

        inline Ref<Rendering::Material> GetRenderMaterial() const {
            return m_using_shared_material ? m_shared_material : m_material;
        }
    };

}