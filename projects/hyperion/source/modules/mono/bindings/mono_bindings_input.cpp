#include "hyppch.hpp"

#include "hyperion/modules/mono/bindings/mono_bindings_input.hpp"

#include <mono/jit/jit.h>

#include "hyperion/core/app/input.hpp"

namespace Hyperion::Scripting {

    bool Binding_IsKeyDown(KeyCode key_code) {
        return Input::IsKeyDown(key_code);
    }

    bool Binding_IsKeyHold(KeyCode key_code) {
        return Input::IsKeyHold(key_code);
    }

    bool Binding_IsKeyUp(KeyCode key_code) {
        return Input::IsKeyUp(key_code);
    }

    bool Binding_IsMouseButtonDown(MouseButtonCode mouse_button_code) {
        return Input::IsMouseButtonDown(mouse_button_code);
    }

    bool Binding_IsMouseButtonHold(MouseButtonCode mouse_button_code) {
        return Input::IsMouseButtonHold(mouse_button_code);
    }

    bool Binding_IsMouseButtonUp(MouseButtonCode mouse_button_code) {
        return Input::IsMouseButtonUp(mouse_button_code);
    }

    void MonoBindingsInput::Bind() {
        mono_add_internal_call("Hyperion.Input::Binding_IsKeyDown", Binding_IsKeyDown);
        mono_add_internal_call("Hyperion.Input::Binding_IsKeyHold", Binding_IsKeyHold);
        mono_add_internal_call("Hyperion.Input::Binding_IsKeyUp", Binding_IsKeyUp);
        mono_add_internal_call("Hyperion.Input::Binding_IsKeyUp", Binding_IsMouseButtonDown);
        mono_add_internal_call("Hyperion.Input::Binding_IsKeyUp", Binding_IsMouseButtonHold);
        mono_add_internal_call("Hyperion.Input::Binding_IsKeyUp", Binding_IsMouseButtonUp);
    }

}