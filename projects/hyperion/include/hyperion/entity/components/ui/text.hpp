#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/entity/components/ui/widget.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Text : public Widget {
        HYP_REFLECT(Widget);
    public:
        String GetText() const { return m_text; }
        void SetText(const String &text);

        Font *GetFont() const { return m_font; }
        void SetFont(Font *font);

        Color GetColor() const { return m_color; }
        void SetColor(Color color);
    protected:
        void OnCreate() override;
    private:
        Text() : Widget("Text") { }
    private:
        void RegenerateMesh();
    private:
        static Mesh *GenerateMeshForText(Font *font, const String &text, Vec2 position, float32 scale, Color color);

        static Text *Create();
    private:
        String m_text;
        Font *m_font = nullptr;
        Color m_color = Color::White();
    };

}