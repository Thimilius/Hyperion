#include "hyppch.hpp"

#include "hyperion/app/input.hpp"

namespace Hyperion {

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