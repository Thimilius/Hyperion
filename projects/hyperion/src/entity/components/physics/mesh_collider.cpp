#include "hyppch.hpp"

#include "hyperion\entity\components\physics\mesh_collider.hpp"

#include "hyperion/entity/world.hpp"
#include "hyperion/physics/physics_world.hpp"

namespace Hyperion {

    void MeshCollider::SetMesh(Rendering::Mesh *mesh) {
        if (m_mesh) {
            GetWorld()->GetPhysicsWorld()->RemoveCollider(this);
        }

        m_mesh = mesh;
        GetWorld()->GetPhysicsWorld()->AddMeshCollider(this);
    }

    void MeshCollider::OnMessage(EntityMessage message) {
        Collider::OnMessage(message);

        if (message.type == EntityMessageType::TransformChanged) {
            if (IsActiveAndEnabled()) {
                GetWorld()->GetPhysicsWorld()->UpdateMeshColliderTransform(this);
            }
        }
    }

}

HYP_REFLECT_REGISTER_BEGIN
{
    registration::class_<MeshCollider>("MeshCollider");
}
HYP_REFLECT_REGISTER_END
