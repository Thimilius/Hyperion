#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"
#include "hyperion/rendering/gizmos/render_gizmo_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderGizmoGrid final {
    public:
        inline static RenderGizmoGridType GetGridType() { return s_type; }
        inline static Mesh *GetGridMesh() { return s_mesh; }
    private:
        static void Initialize();
    private:
        inline static bool8 s_should_draw = true;
        inline static RenderGizmoGridType s_type;
        inline static Mesh *s_mesh;

        inline static const Color GRID_NORMAL_COLOR = Color(0.1f, 0.1f, 0.1f, 0.5f);
        inline static const Color GRID_SPECIAL_COLOR = Color(0.5f, 0.5f, 0.5f, 0.75f);
        inline static const int32 GRID_SIZE = 100;
        inline static const int32 GRID_CHUNK_SIZE = 10;
    private:
        friend class Hyperion::AssetManager;
    };

}