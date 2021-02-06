#include "hyppch.hpp"

#include "hyperion/entity/components/rendering/mesh_renderer.hpp"

#include "hyperion/entity/world.hpp"

namespace Hyperion {

    MeshRenderer *MeshRenderer::Create() {
        return new MeshRenderer();
    }

    void MeshRenderer::OnCreate() {
        GetWorld()->AddMeshRenderer(this);
    }

    void MeshRenderer::OnDestroy() {
        GetWorld()->RemoveMeshRenderer(this);
    }

}

