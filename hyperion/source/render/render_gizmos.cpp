//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/render_gizmos.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/utilities/mesh_builder.hpp"
#include "hyperion/assets/utilities/mesh_generator.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void RenderGizmos::SetTransformationGizmoHighlight(RenderGizmoAxisHighlight highlight) {
    if (s_transformation_gizmo_highlight != highlight) {
      s_transformation_gizmo_highlight = highlight;
      RegenerateTransformationGizmoMesh(highlight);  
    }
  }
  
  //--------------------------------------------------------------
  void RenderGizmos::Initialize() {
    GridMeshGenerationSettings settings;
    settings.size = 100;
    settings.chunk_size = 10;
    settings.normal_color = Color(0.1f, 0.1f, 0.1f, 1.0f);
    settings.chunk_color = Color(0.5f, 0.5f, 0.5f, 1.0f);
    s_grid_mesh = MeshGenerator::GenerateGrid(settings);

    RegenerateTransformationGizmoMesh(RenderGizmoAxisHighlight::None);
  }

  //--------------------------------------------------------------
  void RenderGizmos::RegenerateTransformationGizmoMesh(RenderGizmoAxisHighlight highlight) {
    uint32 vertex_count = 6;
    Rendering::MeshData mesh_data;
    mesh_data.positions.Resize(vertex_count);
    mesh_data.colors.Resize(vertex_count);
    mesh_data.indices.Resize(vertex_count);

    uint32 index = 0;
    mesh_data.positions[0] = Vector3(0.0f, 0.0f, 0.0f);
    mesh_data.colors[0] = highlight == RenderGizmoAxisHighlight::X ? Color::Yellow() : Color::Red();
    mesh_data.indices[0] = index++;
    mesh_data.positions[1] = Vector3(1.0f, 0.0f, 0.0f);
    mesh_data.colors[1] = highlight == RenderGizmoAxisHighlight::X ? Color::Yellow() : Color::Red();
    mesh_data.indices[1] = index++;
    mesh_data.positions[2] = Vector3(0.0f, 0.0f, 0.0f);
    mesh_data.colors[2] = highlight == RenderGizmoAxisHighlight::Y ? Color::Yellow() : Color::Green();
    mesh_data.indices[2] = index++;
    mesh_data.positions[3] = Vector3(0.0f, 1.0f, 0.0f);
    mesh_data.colors[3] = highlight == RenderGizmoAxisHighlight::Y ? Color::Yellow() : Color::Green();
    mesh_data.indices[3] = index++;
    mesh_data.positions[4] = Vector3(0.0f, 0.0f, 0.0f);
    mesh_data.colors[4] = highlight == RenderGizmoAxisHighlight::Z ? Color::Yellow() : Color::Blue();
    mesh_data.indices[4] = index++;
    mesh_data.positions[5] = Vector3(0.0f, 0.0f, 1.0f);
    mesh_data.colors[5] = highlight == RenderGizmoAxisHighlight::Z ? Color::Yellow() : Color::Blue();
    mesh_data.indices[5] = index;
    
    Rendering::SubMeshes sub_meshes = { { Rendering::MeshTopology::Lines, vertex_count, 0, vertex_count, 0 } };
    if (s_transformation_gizmo_mesh) {
      s_transformation_gizmo_mesh->SetData(mesh_data, sub_meshes);
    } else {
      s_transformation_gizmo_mesh = AssetManager::CreateMesh(mesh_data, sub_meshes, AssetDataAccess::ReadAndWrite);
    }
  }

}
