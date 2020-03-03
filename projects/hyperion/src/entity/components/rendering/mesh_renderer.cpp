#include "hyppch.hpp"

#include "hyperion/entity/components/rendering/mesh_renderer.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"

namespace Hyperion {

    void MeshRenderer::OnCreate() {
        Behaviour::OnCreate();

        GetEntity()->GetWorld()->AddMeshRenderer(this);
    }

    void MeshRenderer::OnDestroy() {
        GetEntity()->GetWorld()->RemoveMeshRenderer(this);

        Behaviour::OnDestroy();
    }

}