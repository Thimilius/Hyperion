#pragma once

#include "hyperion/entity/components/entity_component.hpp"
#include "hyperion/rendering/mesh.hpp"
#include "hyperion/rendering/material.hpp"

namespace Hyperion {

    class MeshRendererComponent : public EntityComponent {
    private:
        Ref<Rendering::Mesh> m_mesh;
        Ref<Rendering::Material> m_material;
    public:
        MeshRendererComponent() : EntityComponent("MeshRendererComponent") { }

        Ref<Rendering::Mesh> GetMesh() const { return m_mesh; }
        void SetMesh(const Ref<Rendering::Mesh> &mesh) { m_mesh = mesh; }

        Ref<Rendering::Material> GetMaterial() const { return m_material; }
        void SetMaterial(const Ref<Rendering::Material> &material) { m_material = material; }
    };

}