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
    MeshBuilder mesh_builder;
    mesh_builder.SetTopology(MeshTopology::Lines);
    mesh_builder.AddLine(Vector3::Zero(), Vector3(1.0f, 0.0f, 0.0f), highlight == RenderGizmoAxisHighlight::X ? Color::Yellow() : Color::Red());
    mesh_builder.AddLine(Vector3::Zero(), Vector3(0.0f, 1.0f, 0.0f), highlight == RenderGizmoAxisHighlight::Y ? Color::Yellow() : Color::Green());
    mesh_builder.AddLine(Vector3::Zero(), Vector3(0.0f, 0.0f, 1.0f), highlight == RenderGizmoAxisHighlight::Z ? Color::Yellow() : Color::Blue());
    
    if (s_transformation_gizmo_mesh) {
      mesh_builder.SetToMesh(s_transformation_gizmo_mesh);
    } else {
      s_transformation_gizmo_mesh = mesh_builder.CreateMesh(AssetDataAccess::ReadAndWrite);
    }
  }

}
