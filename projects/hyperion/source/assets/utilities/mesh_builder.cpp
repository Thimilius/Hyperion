//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/utilities/mesh_builder.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void MeshBuilder::Clear() {
        m_mesh_data.positions.clear();
        m_mesh_data.normals.clear();
        m_mesh_data.colors.clear();
        m_mesh_data.texture0.clear();
        m_mesh_data.indices.clear();

        m_index_count = 0;
    }

    //--------------------------------------------------------------
    void MeshBuilder::AddVertex(Vec3 position, Vec3 normal, Vec2 texture0) {
        m_mesh_data.positions.push_back(position);
        m_mesh_data.normals.push_back(normal);
        m_mesh_data.texture0.push_back(texture0);
    }

    //--------------------------------------------------------------
    void MeshBuilder::AddVertex(Vec3 position, Color color, Vec2 texture0) {
        m_mesh_data.positions.push_back(position);
        m_mesh_data.colors.push_back(color);
        m_mesh_data.texture0.push_back(texture0);
    }

    //--------------------------------------------------------------
    void MeshBuilder::AddTriangle(uint32 a, uint32 b, uint32 c) {
        m_mesh_data.indices.push_back(a);
        m_mesh_data.indices.push_back(b);
        m_mesh_data.indices.push_back(c);
        m_index_count += 3;
    }

    //--------------------------------------------------------------
    Mesh *MeshBuilder::CreateMesh() {
        Vector<SubMesh> sub_meshes = { { MeshTopology::Triangles, m_index_count, 0, 0 } };
        return Mesh::Create(m_mesh_data, sub_meshes);
    }

    //--------------------------------------------------------------
    void MeshBuilder::SetToMesh(Mesh *mesh) {
        Vector<SubMesh> sub_meshes = { { MeshTopology::Triangles, m_index_count, 0, 0 } };
        mesh->SetData(m_mesh_data, sub_meshes);
    }

}