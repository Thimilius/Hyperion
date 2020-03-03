#pragma once

#include "hyperion/entity/components/behaviour.hpp"
#include "hyperion/rendering/mesh.hpp"
#include "hyperion/rendering/material.hpp"

namespace Hyperion {

    class MeshRenderer : public Behaviour {
        HYP_OBJECT(MeshRenderer, Behaviour);
    private:
        Ref<Rendering::Mesh> m_mesh;

        Ref<Rendering::Material> m_shared_material;
        mutable Ref<Rendering::Material> m_material;
        mutable bool m_using_shared_material = true;
    public:
        MeshRenderer() : Behaviour("MeshRenderer") { }

        inline Ref<Rendering::Mesh> GetSharedMesh() const { return m_mesh; }
        inline void SetSharedMesh(const Ref<Rendering::Mesh> &mesh) { m_mesh = mesh; }

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
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    };

}