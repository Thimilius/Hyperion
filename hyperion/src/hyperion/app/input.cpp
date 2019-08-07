#include "hyppch.hpp"

#include "hyperion/app/input.hpp"

namespace Hyperion {

    bool CInput::s_keys_down[(u32)EKeyCode::Last];
    bool CInput::s_keys[(u32)EKeyCode::Last];
    bool CInput::s_keys_last[(u32)EKeyCode::Last];
    bool CInput::s_keys_up[(u32)EKeyCode::Last];

    bool CInput::s_mouse_buttons_down[(u32)EMouseButtonCode::Last];
    bool CInput::s_mouse_buttons[(u32)EMouseButtonCode::Last];
    bool CInput::s_mouse_buttons_last[(u32)EMouseButtonCode::Last];
    bool CInput::s_mouse_buttons_up[(u32)EMouseButtonCode::Last];

    Math::SVec2 CInput::s_mouse_position;
    float CInput::s_mouse_scroll;

    void CInput::Update() {
        memset(&s_keys_down, false, sizeof(s_keys_down));
        memset(&s_keys_up, false, sizeof(s_keys_up));
        memcpy(&s_keys_last, &s_keys, sizeof(s_keys_last));

        memset(&s_mouse_buttons_down, false, sizeof(s_mouse_buttons_down));
        memset(&s_mouse_buttons_up, false, sizeof(s_mouse_buttons_up));
        memcpy(&s_mouse_buttons_last, &s_mouse_buttons, sizeof(s_mouse_buttons_last));

        s_mouse_scroll = 0.0f;
    }

}