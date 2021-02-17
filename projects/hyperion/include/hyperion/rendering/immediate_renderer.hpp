#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/rendering/render_driver.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
    class RenderEngine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class ImmediateRenderer final {
    public:
        inline static Material *GetImmediateMaterial() { return s_immediate_material; }

        static void Begin(MeshTopology topolog);
        static void DrawLine(Vec3 start, Vec3 end, Color color);
        static void End();
    private:
        ImmediateRenderer() = delete;
        ~ImmediateRenderer() = delete;
    private:
        static void Initialize(IRenderDriver *render_driver);

        static void AddVertex(Vec3 position, Color color);
    private:
        inline static MeshTopology s_current_topology;
        inline static uint32 s_current_index;

        inline static Mesh *s_immediate_mesh;
        inline static MeshData s_immediate_mesh_data;
        inline static Material *s_immediate_material;

        inline static IRenderDriver *s_render_driver;
    private:
        friend class Hyperion::Rendering::RenderEngine;
    };

}