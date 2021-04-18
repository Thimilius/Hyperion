#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/core/color.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
    class RenderEngine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class ImmediateRenderer final {
    public:
        static void Begin(MeshTopology topolog);
        static void DrawLine(Vec3 start, Vec3 end, Color color);
        static void End();
    private:
        ImmediateRenderer() = delete;
        ~ImmediateRenderer() = delete;
    private:
        static void AddVertex(Vec3 position, Color color);
    private:
        friend class Hyperion::Rendering::RenderEngine;
    };

}