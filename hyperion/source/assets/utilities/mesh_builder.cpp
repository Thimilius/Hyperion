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
  void MeshBuilder::AddQuad(Vector3 corners[4], Vector3 normal) {
    AddVertex(corners[0], normal, Vector2(1.0f, 1.0f));
    AddVertex(corners[1], normal, Vector2(1.0f, 0.0f));
    AddVertex(corners[2], normal, Vector2(0.0f, 0.0f));
    AddVertex(corners[3], normal, Vector2(0.0f, 1.0f));
    AddTriangle(0, 1, 2);
    AddTriangle(0, 2, 3);
    AddIndexOffset(4);
  }

  //--------------------------------------------------------------
  void MeshBuilder::AddQuad(Vector3 corners[4], Vector3 normal, Vector2 uvs[4]) {
    AddVertex(corners[0], normal, uvs[0]);
    AddVertex(corners[1], normal, uvs[1]);
    AddVertex(corners[2], normal, uvs[2]);
    AddVertex(corners[3], normal, uvs[3]);
    AddTriangle(0, 1, 2);
    AddTriangle(0, 2, 3);
    AddIndexOffset(4);
  }

  //--------------------------------------------------------------
  void MeshBuilder::AddQuad(Vector3 corners[4], Color color) {
    AddVertex(corners[0], color, Vector2(1.0f, 1.0f));
    AddVertex(corners[1], color, Vector2(1.0f, 0.0f));
    AddVertex(corners[2], color, Vector2(0.0f, 0.0f));
    AddVertex(corners[3], color, Vector2(0.0f, 1.0f));
    AddTriangle(0, 1, 2);
    AddTriangle(0, 2, 3);
    AddIndexOffset(4);
  }

  //--------------------------------------------------------------
  void MeshBuilder::AddQuad(Vector3 corners[4], Color color, Vector2 uvs[4]) {
    AddVertex(corners[0], color, uvs[0]);
    AddVertex(corners[1], color, uvs[1]);
    AddVertex(corners[2], color, uvs[2]);
    AddVertex(corners[3], color, uvs[3]);
    AddTriangle(0, 1, 2);
    AddTriangle(0, 2, 3);
    AddIndexOffset(4);
  }

  //--------------------------------------------------------------
  void MeshBuilder::Transform(const Matrix4x4 &transformation) {
    for (uint32 i = 0; i < m_mesh_data.positions.GetLength(); i++) {
      Vector3 transformed_position = transformation * m_mesh_data.positions[i];
      m_mesh_data.positions[i] = transformed_position;
    }
  }

  //--------------------------------------------------------------
  void MeshBuilder::TransformAndAlignPixels(const Matrix4x4 &transformation, Vector2Int alignment_size) {
    bool8 align_x = alignment_size.x % 2 != 0;
    bool8 align_y = alignment_size.y % 2 != 0;
    for (uint32 i = 0; i < m_mesh_data.positions.GetLength(); i++) {
      Vector3 transformed_position = transformation * m_mesh_data.positions[i];
      
      float32 x_mod = Math::Abs(Math::FMod(transformed_position.x, 1.0f));
      if (x_mod == 0.5f) {
        transformed_position.x = Math::Ceil(transformed_position.x);
      }
      if (align_x) {
        transformed_position.x += 0.5f;
      }
      
      float32 y_mod = Math::Abs(Math::FMod(transformed_position.y, 1.0f));
      if (y_mod == 0.5f) {
        transformed_position.y = Math::Ceil(transformed_position.y);  
      }
      if (align_y) {
        transformed_position.y += 0.5f;
      }
      
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
