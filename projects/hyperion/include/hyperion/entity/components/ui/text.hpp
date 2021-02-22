#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/entity/components/ui/widget.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Text : public Widget {
        HYP_REFLECT(Widget);
    public:
        inline String GetText() const { return m_text; }
        void SetText(const String &text);

        inline Font *GetFont() const { return m_font; }
        void SetFont(Font *font);
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
    };

}