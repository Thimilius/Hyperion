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
    protected:
        void OnEvent(UIEvent &event) override;
        void DoStateTransition(SelectionState state) override;
    private:
        String m_text;

        UILabel *m_input_label = nullptr;
    };

}