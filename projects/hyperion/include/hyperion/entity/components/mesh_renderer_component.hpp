#pragma once

#include "hyperion/entity/components/behaviour_component.hpp"
#include "hyperion/rendering/mesh.hpp"
#include "hyperion/rendering/material.hpp"

namespace Hyperion {

    class MeshRendererComponent : public BehaviourComponent {
        HYP_OBJECT(MeshRendererComponent, BehaviourComponent);
    private:
        Ref<Rendering::Mesh> m_mesh;
        Ref<Rendering::Material> m_material;
    public:
        MeshRendererComponent() : BehaviourComponent("MeshRendererComponent") { }

        Ref<Rendering::Mesh> GetSharedMesh() const { return m_mesh; }
        void SetSharedMesh(const Ref<Rendering::Mesh> &mesh) { m_mesh = mesh; }

        Ref<Rendering::Material> GetSharedMaterial() const { return m_material; }
        void SetSharedMaterial(const Ref<Rendering::Material> &material) { m_material = material; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    };

}