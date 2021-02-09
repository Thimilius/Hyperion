#include "hyppch.hpp"

#include "hyperion/modules/mono/bindings/mono_bindings_time.hpp"

#include <mono/jit/jit.h>

#include "hyperion/core/app/time.hpp"

namespace Hyperion::Scripting {

    float32 Binding_GetElapsedTime() {
        return Time::GetTime();
    }

    float32 Binding_GetDeltaTime() {
        return Time::GetDeltaTime();
    }

    float32 Binding_GetFixedDeltaTime() {
        return Time::GetFixedDeltaTime();
    }

    void MonoBindingsTime::Bind() {
        mono_add_internal_call("Hyperion.Time::Binding_GetElapsedTime", Binding_GetElapsedTime);
        mono_add_internal_call("Hyperion.Time::Binding_GetDeltaTime", Binding_GetDeltaTime);
        mono_add_internal_call("Hyperion.Time::Binding_GetFixedDeltaTime", Binding_GetFixedDeltaTime);
    }

}