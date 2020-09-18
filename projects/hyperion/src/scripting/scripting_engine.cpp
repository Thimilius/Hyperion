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

        mono_add_internal_call("HyperionEngine.Application::Log", Log);

        char *assemblyPath = "data/managed/HyperionEngine.dll";
        MonoAssembly *assembly = mono_domain_assembly_open(s_root_domain, assemblyPath);
        if (!assembly) {

        }

        MonoImage *image = mono_assembly_get_image(assembly);
        if (!image) {

        }

        MonoMethodDesc *TypeMethodDesc;
        char *TypeMethodDescStr = "Application::Do()";
        TypeMethodDesc = mono_method_desc_new(TypeMethodDescStr, false);
        if (!TypeMethodDesc) {

        }

        //Search the method in the image
        MonoMethod *method = mono_method_desc_search_in_image(TypeMethodDesc, image);
        if (!method) {

        }

        mono_runtime_invoke(method, nullptr, nullptr, nullptr);
    }

    void ScriptingEngine::Shutdown() {
        mono_jit_cleanup(s_root_domain);
    }

}