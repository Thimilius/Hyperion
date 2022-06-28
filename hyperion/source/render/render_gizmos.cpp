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
  void RenderGizmos::UpdateTransformationGizmo(RenderGizmoType type, RenderGizmoAxisHighlight highlight, bool8 dragging) {
    if (s_transformation_gizmo_type != type || s_transformation_gizmo_highlight != highlight || s_transformation_gizmo_dragging != dragging) {
      s_transformation_gizmo_type = type;
      s_transformation_gizmo_highlight = highlight;
      s_transformation_gizmo_dragging = dragging;
      RegenerateTransformationGizmoMesh(type, highlight, dragging);  
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

    RegenerateTransformationGizmoMesh(RenderGizmoType::TRS, RenderGizmoAxisHighlight::None, false);
  }

  //--------------------------------------------------------------
  void RenderGizmos::RegenerateTransformationGizmoMesh(RenderGizmoType type, RenderGizmoAxisHighlight highlight, bool8 dragging) {
    if (type == RenderGizmoType::None) {
      return;
    }
    
    MeshBuilder mesh_builder;
    mesh_builder.SetTopology(MeshTopology::Lines);

    Color highlight_color = Color::Yellow();
    Color non_highlight_color = Color::Grey();
    
    bool8 dragging_x = highlight == RenderGizmoAxisHighlight::X && dragging;
    bool8 dragging_y = highlight == RenderGizmoAxisHighlight::Y && dragging;
    bool8 dragging_z = highlight == RenderGizmoAxisHighlight::Z && dragging;
    
    Color x_color = Color::Red();
    if (highlight == RenderGizmoAxisHighlight::X) {
      x_color = highlight_color;
    }
    if (dragging_y || dragging_z) {
      x_color = non_highlight_color;
    }

    Color y_color = Color::Green();
    if (highlight == RenderGizmoAxisHighlight::Y) {
      y_color = highlight_color;
    }
    if (dragging_x || dragging_z) {
      y_color = non_highlight_color;
    }

    Color z_color = Color::Blue();
    if (highlight == RenderGizmoAxisHighlight::Z) {
      z_color = highlight_color;
    }
    if (dragging_x || dragging_y) {
      z_color = non_highlight_color;
    }
    
    if (type == RenderGizmoType::Translate || type == RenderGizmoType::Scale || type == RenderGizmoType::TRS) {
      mesh_builder.AddLine(Vector3::Zero(), Vector3(1.0f, 0.0f, 0.0f), x_color);
      mesh_builder.AddLine(Vector3::Zero(), Vector3(0.0f, 1.0f, 0.0f), y_color);
      mesh_builder.AddLine(Vector3::Zero(), Vector3(0.0f, 0.0f, 1.0f), z_color);  
    }
    if (type == RenderGizmoType::Rotate || type == RenderGizmoType::TRS) {
      mesh_builder.AddCircle(Vector3::Zero(), 1.0f, Quaternion::FromAxisAngle(Vector3::Up(), 90.0f), x_color);
      mesh_builder.AddCircle(Vector3::Zero(), 1.0f, Quaternion::FromAxisAngle(Vector3::Right(), 90.0f), y_color);
      mesh_builder.AddCircle(Vector3::Zero(), 1.0f, Quaternion::Identity(), z_color);  
    }
    
    if (s_transformation_gizmo_mesh) {
      mesh_builder.SetToMesh(s_transformation_gizmo_mesh);
    } else {
      s_transformation_gizmo_mesh = mesh_builder.CreateMesh(AssetDataAccess::Write);
    }
  }

}
