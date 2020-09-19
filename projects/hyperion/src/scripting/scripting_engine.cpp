#include "hyppch.hpp"

#include "hyperion/scripting/scripting_engine.hpp"

#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Hyperion {

    void Log(MonoString *message) {
        MonoError error;
        char* utf8_message = mono_string_to_utf8_checked(message, &error);
        HYP_TRACE(utf8_message);
        mono_free(utf8_message);
    }

    void ScriptingEngine::Init() {
        mono_set_dirs("data/mono/lib", "data/mono/etc");

        s_root_domain = mono_jit_init("Hyperion");
        if (!s_root_domain) {
            HYP_LOG_ERROR("Scripting", "Initialization of app domain failed!");
            return;
        } else {
            HYP_LOG_INFO("Scripting", "Initialized Mono scripting engine!");
        }
    }

    void ScriptingEngine::Shutdown() {
        mono_jit_cleanup(s_root_domain);
    }

}