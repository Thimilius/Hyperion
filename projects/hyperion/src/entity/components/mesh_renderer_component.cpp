#include "hyppch.hpp"

#include "hyperion/entity/components/mesh_renderer_component.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"

namespace Hyperion {

    void MeshRendererComponent::OnCreate() {
        GetEntity()->GetWorld()->AddMeshRenderer(this);
        
        EntityComponent::OnCreate();
    }

    void MeshRendererComponent::OnDestroy() {
        GetEntity()->GetWorld()->RemoveMeshRenderer(this);

        EntityComponent::OnDestroy();
    }

}