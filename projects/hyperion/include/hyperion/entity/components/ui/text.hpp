#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/entity/components/ui/widget.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class TextAlignment {
        TopLeft,
        TopCenter,
        TopRight,

        MiddleLeft,
        MiddleCenter,
        MiddleRight,

        BottomLeft,
        BottomCenter,
        BottomRight,
    };

    class Text : public Widget {
        HYP_REFLECT(Widget);
    public:
        inline String GetText() const { return m_text; }
        void SetText(const String &text);

        inline Font *GetFont() const { return m_font; }
        void SetFont(Font *font);

        inline TextAlignment GetTextAlignment() const { return m_text_alignment; }
        void SetTextAlignment(TextAlignment text_alignment);
    protected:
        void OnCreate() override;
        void OnRebuildMesh(MeshBuilder &mesh_builder) override;
    private:
        Text() : Widget("Text") { }
    private:
        static Mesh *GenerateMeshForText(MeshBuilder &mesh_builder, Font *font, const String &text, Vec2 position, float32 scale, Color color);

        static Text *Create();
    private:
        String m_text;
        Font *m_font = nullptr;
        TextAlignment m_text_alignment = TextAlignment::MiddleCenter;
    };

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