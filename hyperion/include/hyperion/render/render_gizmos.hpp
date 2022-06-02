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
  
  enum class RenderGizmoGridType {
    XZPlane,
    XYPlane,
    YZPlane,
  };

  class RenderGizmos final {
  public:
    inline static bool8 GetShouldDrawGrid() { return s_should_draw_grid; }
    inline static void SetShouldDrawGrid(bool8 should_draw_grid) { s_should_draw_grid = should_draw_grid; }
    inline static RenderGizmoGridType GetGridType() { return s_grid_type; }
    inline static void SetGridType(RenderGizmoGridType grid_type) { s_grid_type = grid_type; }
    inline static Mesh *GetGridMesh() { return s_grid_mesh; }

    inline static bool8 GetShouldDrawTransformationGizmo() { return s_should_draw_transformation_gizmo; }
    inline static void SetShouldDrawTransformationGizmo(bool8 should_draw_transformation_gizmo) {
      s_should_draw_transformation_gizmo = should_draw_transformation_gizmo;
    }
    inline static Mesh *GetTransformationGizmoMesh() { return s_transformation_gizmo_mesh; }
    inline static Matrix4x4 GetTransformationGizmoTransformation() { return s_transformation_gizmo_transformation; }
    inline static void SetTransformationGizmoTransformation(Matrix4x4 transform) {
      s_transformation_gizmo_transformation = transform;
    }
    static void UpdateTransformationGizmo(RenderGizmoType type, RenderGizmoAxisHighlight highlight);
    
    inline static bool GetShouldDrawAllBounds() { return s_should_draw_all_bounds; }
    inline static void SetShouldDrawAllBounds(bool8 should_draw_all_bounds) { s_should_draw_all_bounds = should_draw_all_bounds; }
  private:
    static void Initialize();

    static void RegenerateTransformationGizmoMesh(RenderGizmoType type, RenderGizmoAxisHighlight highlight);
  private:
    inline static bool8 s_should_draw_grid = true;
    inline static RenderGizmoGridType s_grid_type = RenderGizmoGridType::XZPlane;
    inline static Mesh *s_grid_mesh;

    inline static bool8 s_should_draw_transformation_gizmo = false;
    inline static Mesh *s_transformation_gizmo_mesh;
    inline static Matrix4x4 s_transformation_gizmo_transformation = Matrix4x4::Identity();
    inline static RenderGizmoType s_transformation_gizmo_type = RenderGizmoType::None;
    inline static RenderGizmoAxisHighlight s_transformation_gizmo_highlight = RenderGizmoAxisHighlight::None;

    inline static bool8 s_should_draw_all_bounds = false;
  private:
    friend class Hyperion::Rendering::RenderEngine;
  };

}
