#pragma once

#include "hyperion/entity/components/behaviour.hpp"
#include "hyperion/rendering/material.hpp"

namespace Hyperion {

    class Renderer : public Behaviour {
        RTTR_ENABLE(Behaviour);
    public:
        inline Rendering::Material *GetSharedMaterial() const { return m_shared_material; }
        inline void SetSharedMaterial(Rendering::Material *shared_material) {
            m_shared_material = shared_material;
            m_using_shared_material = true;
        }

        inline Rendering::Material *GetMaterial() const {
            if (m_using_shared_material) {
                m_material = m_shared_material->Copy();
                m_using_shared_material = false;
            }
            return m_material;
        }
        inline void SetMaterial(Rendering::Material *material) {
            m_material = material;
            m_shared_material = material;
        }

        inline Rendering::Material *GetRenderMaterial() const {
            return m_using_shared_material ? m_shared_material : m_material;
        }

        virtual BoundingBox GetBounds() const = 0;
    protected:
        Renderer() = default;
        Renderer(const String & name) : Behaviour(name) {}
        virtual ~Renderer() = default;

        void OnCreate() override;
        void OnDestroy() override;
    private:
        Rendering::Material *m_shared_material;
        mutable Rendering::Material *m_material;
        mutable bool m_using_shared_material = true;
    };

}