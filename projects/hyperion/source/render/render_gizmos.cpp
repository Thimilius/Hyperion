//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/render_gizmos.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/utilities/mesh_generator.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void RenderGizmos::Initialize() {
        GridMeshGenerationSettings settings;
        settings.size = 100;
        settings.chunk_size = 10;
        settings.normal_color = Color(0.1f, 0.1f, 0.1f, 1.0f);
        settings.chunk_color = Color(0.5f, 0.5f, 0.5f, 1.0f);
        s_grid_mesh = MeshGenerator::GenerateGrid(settings);
    }

}