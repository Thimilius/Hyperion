#include "hyppch.hpp"

#include "hyperion/modules/mono/mono_scripting_driver.hpp"

#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/threads.h>

namespace Hyperion {

    void Log(MonoString *message) {
        MonoError error;
        char *utf8_message = mono_string_to_utf8_checked(message, &error);
        HYP_TRACE(utf8_message);
        mono_free(utf8_message);
    }

    void MonoScriptingDriver::Init() {
        mono_set_dirs("data/mono/lib", "data/mono/etc");

        m_root_domain = mono_jit_init("Hyperion");
        if (!m_root_domain) {
            HYP_LOG_ERROR("Scripting", "Initialization of app domain failed!");
            return;
        } else {
            HYP_LOG_INFO("Scripting", "Initialized Mono scripting driver!");
        }

        TestFunctions();
    }

    void MonoScriptingDriver::Shutdown() {
        mono_jit_cleanup(m_root_domain);
    }

    void MonoScriptingDriver::TestFunctions() {
        char *assembly_path = "data/managed/Hyperion.dll";
        MonoAssembly *assembly = mono_domain_assembly_open(m_root_domain, assembly_path);
        MonoImage *image = mono_assembly_get_image(assembly);

        MonoMethodDesc *description = mono_method_desc_new("Hyperion.Application::Do()", true);
        MonoMethod *method = mono_method_desc_search_in_image(description, image);

        mono_add_internal_call("Hyperion.Application::Log", Log);

        mono_runtime_invoke(method, nullptr, nullptr, nullptr);
    }

}