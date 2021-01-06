#pragma once

#include "hyperion/entity/components/ui/widget.hpp"
#include "hyperion/rendering/font.hpp"

namespace Hyperion::UI {

    enum class TextAlignment {
        UpperLeft,
        UpperCenter,
        UpperRight,

        MiddleLeft,
        MiddleCenter,
        MiddleRight,

        LowerLeft,
        LowerCenter,
        LowerRight,
    };

    class Text : public Widget {
        HYP_REFLECT(Widget);
    public:
        inline String GetText() const { return m_text; }
        inline void SetText(const String &text) { m_text = text; }

        inline TextAlignment GetAlignment() const { return m_alignment; }
        inline void SetAlignment(TextAlignment alignment) { m_alignment = alignment; }

        inline Rendering::Font *GetFont() const { return m_font; }
        inline void SetFont(Rendering::Font *font) { m_font = font; }
        inline u32 GetFontSize() const { return m_font_size; }
        void SetFontSize(u32 font_size);
    protected:
        void OnCreate() override;
    private:
        String m_text = "Text";

        TextAlignment m_alignment = TextAlignment::MiddleCenter;

        Rendering::Font *m_font;
        u32 m_font_size = 16;
    };

}