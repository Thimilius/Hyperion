#include "hyppch.hpp"

#include "hyperion/entity/components/mesh_renderer.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"

namespace Hyperion {

    void MeshRenderer::OnCreate() {
        GetEntity()->GetWorld()->AddMeshRenderer(this);
        
        Component::OnCreate();
    }

    void MeshRenderer::OnDestroy() {
        GetEntity()->GetWorld()->RemoveMeshRenderer(this);

        Component::OnDestroy();
    }

}