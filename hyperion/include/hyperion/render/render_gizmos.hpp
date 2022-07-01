//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
  class RenderEngine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  enum class RenderGizmoType {
    None,
    Translate,
    Rotate,
    Scale,
    TRS
  };
  
  enum class RenderGizmoAxisHighlight {
    None,
    X,
    Y,
    Z
  };
  
  class RenderGizmos final {
  public:
    inline static bool8 GetShouldDrawGrid() { return s_should_draw_grid; }
    inline static void SetShouldDrawGrid(bool8 should_draw_grid) { s_should_draw_grid = should_draw_grid; }
    inline static Mesh *GetGridMesh() { return s_grid_mesh; }

    inline static bool GetShouldDrawHighlight() { return s_should_draw_highlight; }
    inline static void SetShouldDrawHighlight(bool8 should_draw_highlight) { s_should_draw_highlight = should_draw_highlight; }
    inline static Mesh *GetHighlightMesh() { return s_highlight_mesh; }
    inline static void SetHighlightMesh(Mesh *highlight_mesh) { s_highlight_mesh = highlight_mesh; }
    inline static Matrix4x4 GetHighlightTransformation() { return s_highlight_transformation; }
    inline static void SetHighlightTransformation(Matrix4x4 transform) { s_highlight_transformation = transform; }
    
    inline static bool8 GetShouldDrawTransformationGizmo() { return s_should_draw_transformation_gizmo; }
    inline static void SetShouldDrawTransformationGizmo(bool8 should_draw_transformation_gizmo) {
      s_should_draw_transformation_gizmo = should_draw_transformation_gizmo;
    }
    inline static Mesh *GetTransformationGizmoMesh() { return s_transformation_gizmo_mesh; }
    inline static Matrix4x4 GetTransformationGizmoTransformation() { return s_transformation_gizmo_transformation; }
    inline static void SetTransformationGizmoTransformation(Matrix4x4 transform) { s_transformation_gizmo_transformation = transform; }
    static void UpdateTransformationGizmo(RenderGizmoType type, RenderGizmoAxisHighlight highlight, bool8 dragging);
    
    inline static bool GetShouldDrawMeshBounds() { return s_should_draw_mesh_bounds; }
    inline static void SetShouldDrawMeshBounds(bool8 should_draw_mesh_bounds) { s_should_draw_mesh_bounds = should_draw_mesh_bounds; }
  private:
    static void Initialize();

    static void RegenerateTransformationGizmoMesh(RenderGizmoType type, RenderGizmoAxisHighlight highlight, bool8 dragging);
  private:
    inline static bool8 s_should_draw_grid = false;
    inline static Mesh *s_grid_mesh;

    inline static bool8 s_should_draw_highlight = false;
    inline static Mesh *s_highlight_mesh;
    inline static Matrix4x4 s_highlight_transformation = Matrix4x4::Identity();
    
    inline static bool8 s_should_draw_transformation_gizmo = false;
    inline static Mesh *s_transformation_gizmo_mesh;
    inline static Matrix4x4 s_transformation_gizmo_transformation = Matrix4x4::Identity();
    inline static RenderGizmoType s_transformation_gizmo_type = RenderGizmoType::None;
    inline static RenderGizmoAxisHighlight s_transformation_gizmo_highlight = RenderGizmoAxisHighlight::None;
    inline static bool8 s_transformation_gizmo_dragging = false;

    inline static bool8 s_should_draw_mesh_bounds = false;
  private:
    friend class Hyperion::Rendering::RenderEngine;
  };

}
