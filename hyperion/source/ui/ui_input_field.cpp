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

        MarkDirty();
      }
    }
  }

  //--------------------------------------------------------------
  void UIInputField::SetInputLabel(UILabel *input_label) {
    if (m_input_label != input_label) {
      m_input_label = input_label;

      MarkDirty();
    }
  }

  //--------------------------------------------------------------
  void UIInputField::SetLineType(InputFieldLineType line_type) {
    if (m_line_type != line_type) {
      m_line_type = line_type;

      MarkDirty();
    }
  }

  //--------------------------------------------------------------
  void UIInputField::SetCaretWidth(float32 caret_width) {
    if (m_caret_width != caret_width) {
      m_caret_width = caret_width;

      MarkDirty();
    }
  }

  //--------------------------------------------------------------
  void UIInputField::SetCaretColor(Color caret_color) {
    if (m_caret_color != caret_color) {
      m_caret_color = caret_color;

      MarkDirty();
    }
  }

  //--------------------------------------------------------------
  void UIInputField::OnEvent(UIEvent &event) {
    UIElement::OnEvent(event);

    if (event.GetType() == UIEventType::Select) {
      MarkDirty();
    } else if (event.GetType() == UIEventType::SelectUpdate) { } else if (event.GetType() == UIEventType::Deselect) {
      MarkDirty();
    } else if (event.GetType() == UIEventType::Submit) {
      MarkDirty();
    } else if (event.GetType() == UIEventType::Cancel) {
      SetText("");
    } else if (event.GetType() == UIEventType::KeyPressed) {
      KeyCode key_code = event.GetKeyCode();

      if (key_code == KeyCode::Delete) {
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
    } else if (event.GetType() == UIEventType::KeyTyped) {
      if (!m_input_label) {
        return;
      }

      KeyCode key_code = event.GetKeyCode();
      String key_typed = event.GetKeyTyped();

      if (key_code == KeyCode::Back) {
        String text = GetText();
        if (!text.empty()) {
          uint32 codepoint_size = StringUtils::GetLastUtf8CodepointSize(text);
          text.resize(text.size() - codepoint_size);
          SetText(text);
        }
        return;
      } else if (key_code == KeyCode::Return) {
        if (m_line_type == InputFieldLineType::SingleLine) {
          SetText("");
        } else if (m_line_type == InputFieldLineType::MultiLine) {
          SetText(GetText() + "\n");
        }
        return;
      } else if (key_code == KeyCode::Escape) {
        return;
      }

      if ((event.GetKeyModifier() & KeyModifier::Control) == KeyModifier::Control) {
        if (key_code == KeyCode::A) {
          return;
        } else if (key_code == KeyCode::C) {
          return;
        } else if (key_code == KeyCode::V) {
          return;
        } else if (key_code == KeyCode::X) {
          return;
        }
      }

      Font *font = m_input_label->GetFont();
      if (font) {
        bool8 has_characters = true;
        Array<uint32> codepoints = StringUtils::GetCodepointsFromUtf8(key_typed);
        for (uint32 codepoint : codepoints) {
          if (codepoint == ' ' || codepoint == '\t') {
            continue;
          } else if (!font->HasCodepoint(codepoint)) {
            has_characters = false;
          }
        }

        if (has_characters) {
          SetText(GetText() + key_typed);
        }
      }
    }
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

    Vector3 top_left = corners[3];
    Vector3 bottom_left = corners[2];
    float32 local_size = top_left.y - bottom_left.y;
    float32 font_size = font->GetSize() + 4.0f;

    if (local_size >= font_size) {
      corners[0] = Vector3(top_left.x + m_caret_width, top_left.y, 0.0f);
      corners[1] = Vector3(bottom_left.x + m_caret_width, bottom_left.y, 0.0f);
      corners[2] = Vector3(bottom_left, 0.0f);
      corners[3] = Vector3(top_left, 0.0f);

      AddQuad(mesh_builder, corners, m_caret_color);
    }
  }

}
