//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_input_field.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    //--------------------------------------------------------------
    UIInputField::UIInputField() {
        SetRaycastTarget(true);
    }

    //--------------------------------------------------------------
    void UIInputField::SetText(const String &text) {
        if (m_text != text) {
            m_text = text;

            if (m_input_label) {
                m_input_label->SetText(text);
            }
        }
    }

    //--------------------------------------------------------------
    void UIInputField::SetInputLabel(UILabel *input_label) {
        if (m_input_label != input_label) {
            m_input_label = input_label;
        }
    }

    //--------------------------------------------------------------
    void UIInputField::OnEvent(UIEvent &event) {
        UIElement::OnEvent(event);

        if (event.GetType() == UIEventType::Select) {

        } else if (event.GetType() == UIEventType::SelectUpdate) {

        } else if (event.GetType() == UIEventType::Deselect) {

        } else if (event.GetType() == UIEventType::KeyTyped) {
            if (m_input_label) {
                KeyCode key_code = event.GetKeyCode();
                String key_typed = event.GetKeyTyped();

                if (key_code == KeyCode::Back) {
                    String text = GetText();
                    if (text != "") {
                        // FIXME: This is not compatible with utf-8!
                        text.resize(text.size() - 1);
                        SetText(text);
                    }
                    return;
                } else if (key_code == KeyCode::Delete) {
                    return;
                } else if (key_code == KeyCode::Return) {
                    return;
                } else if (key_code == KeyCode::Left) {
                    return;
                } else if (key_code == KeyCode::Right) {
                    return;
                } else if (key_code == KeyCode::Up) {
                    return;
                } else if (key_code == KeyCode::Down) {
                    return;
                } else if (key_code == KeyCode::Home) {
                    return;
                } else if (key_code == KeyCode::End) {
                    return;
                }

                if ((event.GetKeyModifier() & KeyModifier::Control) == KeyModifier::Control) {
                    if (key_code == KeyCode::A) {

                    } else if (key_code == KeyCode::C) {

                    } else if (key_code == KeyCode::V) {

                    } else if (key_code == KeyCode::X) {

                    }
                    return;
                }

                SetText(GetText() + key_typed);
            }
        }

        MarkDirty();
    }

    //--------------------------------------------------------------
    void UIInputField::DoStateTransition(SelectionState state) {
        // We overwrite the pressed state when we are already selected.
        // "Pressing" an already selected input field does not make any sense.
        if (state == SelectionState::Pressed && IsSelected()) {
            state = SelectionState::Selected;
        }

        UIElement::DoStateTransition(state);
    }

    //--------------------------------------------------------------
    void UIInputField::OnRebuildGeometry(MeshBuilder &mesh_builder) {
        UIElement::OnRebuildGeometry(mesh_builder);

        if (!IsSelected()) {
            return;
        }
        if (m_input_label == nullptr) {
            return;
        }
        Font *font = m_input_label->GetFont();
        if (font == nullptr) {
            return;
        }

        Vector3 corners[4];
        GetLocalCorners(corners);

        Vector3 top_left = corners[static_cast<uint32>(Corner::TopLeft)];
        Vector3 bottom_left = corners[static_cast<uint32>(Corner::BottomLeft)];
        float32 local_size = top_left.y - bottom_left.y;
        float32 font_size = font->GetSize() + 4.0f;
        float32 caret_width = 2.0f;

        if (local_size >= font_size) {
            corners[static_cast<uint32>(Corner::TopRight)] = Vector3(top_left.x + caret_width, top_left.y, 0.0f);
            corners[static_cast<uint32>(Corner::BottomRight)] = Vector3(bottom_left.x + caret_width, bottom_left.y, 0.0f);
            corners[static_cast<uint32>(Corner::BottomLeft)] = Vector3(bottom_left, 0.0f);
            corners[static_cast<uint32>(Corner::TopLeft)] = Vector3(top_left, 0.0f);
            
            AddQuad(mesh_builder, corners, Color::White());
        }
    }

}