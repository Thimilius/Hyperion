//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/gizmos/render_gizmo_grid.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void RenderGizmoGrid::Initialize() {
        int32 half_grid_size = GRID_SIZE / 2;
        float32 to_point = static_cast<float32>(half_grid_size);

        // We have to remember that we are actually drawing always one more line that the actual size of the grid.
        uint32 grid_vertex_count = ((GRID_SIZE + 1) * 4);
        MeshData mesh_data;
        mesh_data.positions.Resize(grid_vertex_count);
        mesh_data.colors.Resize(grid_vertex_count);
        mesh_data.indices.Resize(grid_vertex_count);

        uint32 index = 0;
        for (int32 x = -half_grid_size; x <= half_grid_size; x++) {
            float32 from_point = static_cast<float32>(x);
            Color color = (x % GRID_CHUNK_SIZE) == 0 ? GRID_SPECIAL_COLOR : GRID_NORMAL_COLOR;
            mesh_data.positions[index] = Vector3(from_point, 0, to_point);
            mesh_data.colors[index] = color;
            mesh_data.indices[index] = index;
            index++;
            mesh_data.positions[index] = Vector3(from_point, 0, -to_point);
            mesh_data.colors[index] = color;
            mesh_data.indices[index] = index;
            index++;
        }
        for (int32 z = -half_grid_size; z <= half_grid_size; z++) {
            float32 from_point = static_cast<float32>(z);
            Color color = (z % GRID_CHUNK_SIZE) == 0 ? GRID_SPECIAL_COLOR : GRID_NORMAL_COLOR;
            mesh_data.positions[index] = Vector3(to_point, 0, from_point);
            mesh_data.colors[index] = color;
            mesh_data.indices[index] = index;
            index++;
            mesh_data.positions[index] = Vector3(-to_point, 0, from_point);
            mesh_data.colors[index] = color;
            mesh_data.indices[index] = index;
            index++;
        }

        Array<SubMesh> sub_meshes = { { MeshTopology::Lines, grid_vertex_count, 0, 0 } };
        s_mesh = AssetManager::CreateMesh(mesh_data, sub_meshes);
    }

}