//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/ui/ui_element.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    class UILabel : public UIElement {
        HYP_OBJECT(UILabel);
    public:
        UILabel();
    public:
        inline Font *GetFont() const { return m_font; }
        void SetFont(Font *font);

        inline String GetText() const { return m_text; }
        void SetText(const String &text);

        inline UI::TextAlignment GetTextAlignment() const { return m_alignment; }
        void SetAlignment(UI::TextAlignment alignment);
    protected:
        void OnRebuildGeometry(MeshBuilder &mesh_builder) override;
    private:
        Font *m_font = nullptr;
        String m_text;
        UI::TextAlignment m_alignment = UI::TextAlignment::MiddleCenter;
    };

}