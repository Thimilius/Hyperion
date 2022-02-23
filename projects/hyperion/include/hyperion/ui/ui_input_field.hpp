//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ui/ui_element.hpp"
#include "hyperion/ui/ui_label.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  class UIInputField : public UIElement {
    HYP_OBJECT(UIInputField);
  public:
    UIInputField();
  public:
    inline String GetText() const { return m_text; }
    void SetText(const String &text);

    inline UILabel *GetInputLabel() const { return m_input_label; }
    void SetInputLabel(UILabel *input_label);

    inline InputFieldLineType GetLineType() const { return m_line_type; }
    void SetLineType(InputFieldLineType line_type);

    inline float32 GetCaretWidth() const { return m_caret_width; }
    void SetCaretWidth(float32 caret_width);
    inline Color GetCaretColor() const { return m_caret_color; }
    void SetCaretColor(Color caret_color);
  protected:
    void OnEvent(UIEvent &event) override;
    void DoStateTransition(SelectionState state) override;

    void OnRebuildGeometry(MeshBuilder &mesh_builder) override;
  private:
    String m_text;

    InputFieldLineType m_line_type = InputFieldLineType::SingleLine;

    UILabel *m_input_label = nullptr;

    float32 m_caret_width = 1.0f;
    Color m_caret_color = Color::White();
  };

}