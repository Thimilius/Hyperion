#pragma once

#include "hyperion/entity/components/behaviour.hpp"
#include "hyperion/rendering/mesh.hpp"
#include "hyperion/rendering/material.hpp"

namespace Hyperion {

    class MeshRenderer : public Behaviour {
        HYP_OBJECT(MeshRenderer, Behaviour);
    private:
        Ref<Rendering::Mesh> m_mesh;
        Ref<Rendering::Material> m_material;
    public:
        MeshRenderer() : Behaviour("MeshRenderer") { }

        inline Ref<Rendering::Mesh> GetSharedMesh() const { return m_mesh; }
        inline void SetSharedMesh(const Ref<Rendering::Mesh> &mesh) { m_mesh = mesh; }

        inline Ref<Rendering::Material> GetSharedMaterial() const { return m_material; }
        inline void SetSharedMaterial(const Ref<Rendering::Material> &material) { m_material = material; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    };

}