#pragma once

#include <hyperion/assets/material.hpp>
#include <hyperion/assets/mesh.hpp>
#include <hyperion/rendering/render_driver.hpp>

namespace Hyperion::Editor {

    enum class EditorWorldGridType {
        XZPlane,
        XYPlane,
        YZPlane,
    };

    class EditorWorldGrid final {
    public:
        static void Initialize();
        static void Render(Rendering::IRenderDriver *render_driver, const Rendering::CameraData &camera_data);
    private:
        EditorWorldGrid() = delete;
        ~EditorWorldGrid() = delete;
    private:
        static Mat4 GetTranslation(const Rendering::CameraData &camera_data);
        static Mat4 GetRotation();
    private:
        inline static EditorWorldGridType s_grid_type = EditorWorldGridType::XZPlane;
        inline static Mesh *s_mesh;
        inline static Material *s_material;

        inline static const Color GRID_COLOR = Color(0.15f, 0.15f, 0.15f, 0.75f);
        inline static const Color GRID_SPECIAL_COLOR = Color(0.6f, 0.6f, 0.6f, 0.75f);
        inline static const int32 GRID_SIZE = 100;
        inline static const int32 GRID_CHUNK_SIZE = 10;
    };

}