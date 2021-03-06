#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/utilities/mesh_builder.hpp"
#include "hyperion/assets/utilities/text_alignment.hpp"
#include "hyperion/core/math/rect.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct TextMeshGenerationSettings {
        String text = "";
        Font *font = nullptr;
        TextAlignment alignment = TextAlignment::MiddleCenter;
        Color color = Color::White();

        Rect rect = Rect();
        float32 scale = 1.0f;
    };

    class TextMeshGenerator final {
    public:
        static Mesh *GenerateMesh(const TextMeshGenerationSettings &settings);
        static Mesh *GenerateMesh(const TextMeshGenerationSettings &settings, MeshBuilder &mesh_builder);
    private:
        static Vec2 GetPosition(TextAlignment text_alignment, Vec2 text_size, Rect rect);
    private:
        TextMeshGenerator() = delete;
        ~TextMeshGenerator() = delete;
    };

}