//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"

//---------------------- Project Includes ----------------------
namespace Hyperion::Rendering {
  class RenderEngine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

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

    inline static bool GetShouldDrawAllBounds() { return s_should_draw_all_bounds; }
    inline static void SetShouldDrawAllBounds(bool8 should_draw_all_bounds) { s_should_draw_all_bounds = should_draw_all_bounds; }
  private:
    static void Initialize();
  private:
    inline static bool8 s_should_draw_grid = true;
    inline static RenderGizmoGridType s_grid_type = RenderGizmoGridType::XZPlane;
    inline static Mesh *s_grid_mesh;

    inline static bool8 s_should_draw_all_bounds = false;
  private:
    friend class Hyperion::Rendering::RenderEngine;
  };

}
