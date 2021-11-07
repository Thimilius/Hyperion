//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/utilities/mesh_builder.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void MeshBuilder::AddVertex(Vector3 position, Vector3 normal, Vector2 texture0) {
        m_mesh_data.positions.Add(position);
        m_mesh_data.normals.Add(normal);
        m_mesh_data.texture0.Add(texture0);

        m_vertex_count += 1;
    }

    //--------------------------------------------------------------
    void MeshBuilder::AddVertex(Vector3 position, Color color, Vector2 texture0) {
        m_mesh_data.positions.Add(position);
        m_mesh_data.colors.Add(color);
        m_mesh_data.texture0.Add(texture0);

        m_vertex_count += 1;
    }

    //--------------------------------------------------------------
    void MeshBuilder::AddTriangle(uint32 a, uint32 b, uint32 c) {
        m_mesh_data.indices.Add(m_index_offset + a);
        m_mesh_data.indices.Add(m_index_offset + b);
        m_mesh_data.indices.Add(m_index_offset + c);
        m_index_count += 3;
    }

    //--------------------------------------------------------------
    void MeshBuilder::Transform(const Matrix4x4 &transformation) {
        for (uint32 i = 0; i < m_mesh_data.positions.GetLength(); i++) {
            Vector3 transformed_position = transformation * m_mesh_data.positions[i];
            m_mesh_data.positions[i] = transformed_position;
        }
    }

    //--------------------------------------------------------------
    Mesh *MeshBuilder::CreateMesh() {
        if (IsEmpty()) {
            return nullptr;
        } else {
            Array<SubMesh> sub_meshes = { { Rendering::MeshTopology::Triangles, m_vertex_count, 0, m_index_count, 0 } };
            return AssetManager::CreateMesh(m_mesh_data, sub_meshes);
        }
    }

    //--------------------------------------------------------------
    void MeshBuilder::SetToMesh(Mesh *mesh) {
        Array<SubMesh> sub_meshes = { { Rendering::MeshTopology::Triangles, m_vertex_count, 0, m_index_count, 0 } };
        mesh->SetData(m_mesh_data, sub_meshes);
    }

    //--------------------------------------------------------------
    void MeshBuilder::Clear() {
        m_mesh_data.positions.Clear();
        m_mesh_data.normals.Clear();
        m_mesh_data.colors.Clear();
        m_mesh_data.texture0.Clear();
        m_mesh_data.indices.Clear();

        m_vertex_count = 0;
        m_index_count = 0;
        m_index_offset = 0;
    }

}