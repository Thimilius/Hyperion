//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/ui/ui_element.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  class UILabel : public UIElement {
    HYP_REFLECT(UIElement)
  public:
    UILabel();
  public:
    inline Font *GetFont() const { return m_font; }
    void SetFont(Font *font);

    inline String GetText() const { return m_text; }
    void SetText(const String &text);

    inline UI::TextAlignment GetTextAlignment() const { return m_alignment; }
    void SetTextAlignment(UI::TextAlignment alignment);
  protected:
    void OnRebuildShadowGeometry(MeshBuilder &mesh_builder, Color shadow_color, Vector2 shadow_offset) override;
    void OnRebuildGeometry(MeshBuilder &mesh_builder) override;
  private:
    void RebuildTextGeometry(MeshBuilder &mesh_builder, Color color, Rect rect);
    bool8 ValidateGeometryToRebuild();
  private:
    Font *m_font = nullptr;
    String m_text;
    UI::TextAlignment m_alignment = UI::TextAlignment::MiddleCenter;
  };

}
