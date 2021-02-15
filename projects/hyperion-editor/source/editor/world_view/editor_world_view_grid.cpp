//--------------------- Definition Include ---------------------
#include "hyperion/editor/world_view/editor_world_view_grid.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/assets/shader.hpp>
#include <hyperion/core/io/file_system.hpp>
#include <hyperion/rendering/immediate_renderer.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/world_view/editor_world_view.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorWorldViewGrid::Initialize() {
        int32 half_grid_size = GRID_SIZE / 2;
        float32 to_point = static_cast<float32>(half_grid_size);

        // We have to remember that we are actually drawing always one more line that the actual size of the grid.
        uint32 grid_vertex_count = ((GRID_SIZE + 1) * 4);
        MeshData mesh_data;
        mesh_data.positions.resize(grid_vertex_count);
        mesh_data.colors.resize(grid_vertex_count);
        mesh_data.indices.resize(grid_vertex_count);

        uint32 index = 0;
        for (int32 x = -half_grid_size; x <= half_grid_size; x++) {
            float32 from_point = static_cast<float32>(x);
            Color color = (x % GRID_CHUNK_SIZE) == 0 ? GRID_SPECIAL_COLOR : GRID_COLOR;
            mesh_data.positions[index] = Vec3(from_point, 0, to_point);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = color;
            index++;
            mesh_data.positions[index] = Vec3(from_point, 0, -to_point);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = color;
            index++;
        }
        for (int32 z = -half_grid_size; z <= half_grid_size; z++) {
            float32 from_point = static_cast<float32>(z);
            Color color = (z % GRID_CHUNK_SIZE) == 0 ? GRID_SPECIAL_COLOR : GRID_COLOR;
            mesh_data.positions[index] = Vec3(to_point, 0, from_point);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = color;
            index++;
            mesh_data.positions[index] = Vec3(-to_point, 0, from_point);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = color;
            index++;
        }

        Vector<SubMesh> sub_meshes = { { MeshTopology::Lines, grid_vertex_count, 0, 0 } };
        s_mesh = Mesh::Create(mesh_data, sub_meshes);
    }

    //--------------------------------------------------------------
    void EditorWorldViewGrid::Render(IRenderDriver *render_driver, Vec3 target_position) {
        // We want to draw the grid at the center of the camera corresponding to the grid chunk size.
        Mat4 translation = GetTranslation(target_position);
        Mat4 rotation = GetRotation();
        render_driver->DrawMesh(s_mesh->GetResourceId(), translation * rotation, ImmediateRenderer::GetImmediateMaterial()->GetResourceId(), 0);
    }

    //--------------------------------------------------------------
    Mat4 EditorWorldViewGrid::GetTranslation(Vec3 target_position) {
        int32 x = static_cast<int32>(target_position.x + (GRID_CHUNK_SIZE / 2.0f)) / GRID_CHUNK_SIZE;
        int32 y = static_cast<int32>(target_position.y + (GRID_CHUNK_SIZE / 2.0f)) / GRID_CHUNK_SIZE;
        int32 z = static_cast<int32>(target_position.z + (GRID_CHUNK_SIZE / 2.0f)) / GRID_CHUNK_SIZE;
        Vec3 grid_position;
        switch (s_grid_type) {
            case EditorWorldViewGridType::XZPlane: return Mat4::Translate(static_cast<float32>(x * GRID_CHUNK_SIZE), 0.0f, static_cast<float32>(z * GRID_CHUNK_SIZE));
            case EditorWorldViewGridType::XYPlane: return Mat4::Translate(static_cast<float32>(x * GRID_CHUNK_SIZE), static_cast<float32>(y * GRID_CHUNK_SIZE), 0.0f);
            case EditorWorldViewGridType::YZPlane: return Mat4::Translate(0.0f, static_cast<float32>(y * GRID_CHUNK_SIZE), static_cast<float32>(z * GRID_CHUNK_SIZE));
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return Mat4::Identity();
        }
    }

    //--------------------------------------------------------------
    Mat4 EditorWorldViewGrid::GetRotation() {
        switch (s_grid_type) {
            case EditorWorldViewGridType::XZPlane: return Mat4::Identity();
            case EditorWorldViewGridType::XYPlane: return Mat4::Rotate(Vec3::Right(), 90.0f);
            case EditorWorldViewGridType::YZPlane: return Mat4::Rotate(Vec3::Forward(), 90.0f);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return Mat4::Identity();
        }
    }

}