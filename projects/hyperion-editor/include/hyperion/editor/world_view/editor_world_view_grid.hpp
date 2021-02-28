#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/assets/mesh.hpp>
#include <hyperion/rendering/render_driver.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    enum class EditorWorldViewGridType {
        XZPlane,
        XYPlane,
        YZPlane,
    };

    class EditorWorldViewGrid final {
    public:
        static void Initialize();
        static void Render(Rendering::IRenderDriver *render_driver, Vec3 target_position);
    private:
        EditorWorldViewGrid() = delete;
        ~EditorWorldViewGrid() = delete;
    private:
        static Mat4 GetTranslation(Vec3 target_position);
        static Mat4 GetRotation();
    private:
        inline static EditorWorldViewGridType s_grid_type = EditorWorldViewGridType::XZPlane;
        inline static Mesh *s_mesh;

        inline static const Color GRID_COLOR = Color(0.1f, 0.1f, 0.1f, 0.5f);
        inline static const Color GRID_SPECIAL_COLOR = Color(0.5f, 0.5f, 0.5f, 0.75f);
        inline static const int32 GRID_SIZE = 100;
        inline static const int32 GRID_CHUNK_SIZE = 10;
    };

}