#include "hyppch.hpp"

#include "hyperion/entity/components/rendering/mesh_renderer.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"

namespace Hyperion {

    BoundingBox MeshRenderer::GetBounds() const {
        BoundingBox box = GetRenderMesh()->GetBounds();
        Mat4 transform = GetTransform()->GetLocalToWorldMatrix();
        Vec3 mesh_min = box.min;
        Vec3 mesh_max = box.max;
        Vector<Vec3> positions = {
            transform * Vec3(mesh_max.x, mesh_max.y, mesh_min.z),
            transform * Vec3(mesh_max.x, mesh_min.y, mesh_min.z),
            transform * Vec3(mesh_min.x, mesh_min.y, mesh_min.z),
            transform * Vec3(mesh_min.x, mesh_max.y, mesh_min.z),

            transform * Vec3(mesh_max.x, mesh_max.y, mesh_max.z),
            transform * Vec3(mesh_max.x, mesh_min.y, mesh_max.z),
            transform * Vec3(mesh_min.x, mesh_min.y, mesh_max.z),
            transform * Vec3(mesh_min.x, mesh_max.y, mesh_max.z),
        };

        Vec3 min = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        Vec3 max = Vec3(FLT_MIN, FLT_MIN, FLT_MIN);
        for (u32 i = 0; i < positions.size(); i++) {
            Vec3 position = positions[i];
            if (position.x < min.x) {
                min.x = position.x;
            }
            if (position.y < min.y) {
                min.y = position.y;
            }
            if (position.z < min.z) {
                min.z = position.z;
            }
            if (position.x > max.x) {
                max.x = position.x;
            }
            if (position.y > max.y) {
                max.y = position.y;
            }
            if (position.z > max.z) {
                max.z = position.z;
            }
        }

        return BoundingBox(min, max);
    }

    void MeshRenderer::OnCreate() {
        Renderer::OnCreate();

        GetWorld()->AddMeshRenderer(this);
    }

    void MeshRenderer::OnDestroy() {
        GetWorld()->RemoveMeshRenderer(this);

        Renderer::OnDestroy();
    }

}

HYP_REFLECT_REGISTER_BEGIN
{
    registration::class_<MeshRenderer>("MeshRenderer");
}
HYP_REFLECT_REGISTER_END
