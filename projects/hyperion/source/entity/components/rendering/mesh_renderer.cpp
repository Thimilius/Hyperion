//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    MeshRenderer *MeshRenderer::Create() {
        return new MeshRenderer();
    }

    //--------------------------------------------------------------
    void MeshRenderer::OnCreate() {
        GetWorld()->AddMeshRenderer(this);
    }

    //--------------------------------------------------------------
    void MeshRenderer::OnDestroy() {
        GetWorld()->RemoveMeshRenderer(this);
    }

}

