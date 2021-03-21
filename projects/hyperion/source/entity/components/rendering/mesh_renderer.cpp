//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void MeshRenderer::OnCreate() {
        GetWorld()->AddMeshRenderer(this);
    }

    //--------------------------------------------------------------
    void MeshRenderer::OnDestroy() {
        GetWorld()->RemoveMeshRenderer(this);
    }

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(MeshRenderer)
    HYP_REFLECT_BASE(Renderer)
    HYP_REFLECT_CONSTRUCTOR([]() { return new MeshRenderer(); })
    HYP_REFLECT_END()

}

