#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/ui/ui_element.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    class UILabel : public UIElement {
    public:
        inline Font *GetFont() const { return m_font; }
        void SetFont(Font *font);

        inline String GetText() const { return m_text; }
        void SetText(const String &text);

        inline UI::UITextAlignment GetTextAlignment() const { return m_alignment; }
        void SetAlignment(UI::UITextAlignment alignment);
    protected:
        void OnRebuildGeometry(MeshBuilder &mesh_builder) override;
    private:
        Font *m_font = nullptr;
        String m_text;
        UI::UITextAlignment m_alignment = UI::UITextAlignment::MiddleCenter;
    };

}