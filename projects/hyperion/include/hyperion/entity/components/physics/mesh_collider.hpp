#pragma once

#include "hyperion/entity/components/physics/collider.hpp"
#include "hyperion/rendering/mesh.hpp"

namespace Hyperion {

    class MeshCollider final : public Collider {
        HYP_OBJECT(MeshCollider, Collider);
    public:
        MeshCollider() : Collider("MeshCollider") { }

        ColliderType GetColliderType() const { return ColliderType::MeshCollider; }

        void SetMesh(Rendering::Mesh *mesh);
        Rendering::Mesh *GetMesh() const { return m_mesh; }

        void OnMessage(EntityMessage message) override;
    private:
        Rendering::Mesh *m_mesh = nullptr;
    };

}