#include "hyppch.hpp"

#include "hyperion/modules/mono/mono_scripting_driver.hpp"

#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/threads.h>

#include "hyperion/modules/mono/bindings/mono_bindings_entity.hpp"
#include "hyperion/modules/mono/bindings/mono_bindings_input.hpp"
#include "hyperion/modules/mono/bindings/mono_bindings_object.hpp"
#include "hyperion/modules/mono/bindings/mono_bindings_time.hpp"

namespace Hyperion::Scripting {

    void MonoScriptingDriver::Initialize(const ScriptingSettings &settings) {
        InitDebugger(settings);
        InitDomain();
        InitBindings();
        InitAssembly();
        InitMethods();
    }

    void MonoScriptingDriver::Update() {
        mono_runtime_invoke(s_update_method, nullptr, nullptr, nullptr);
    }

    void MonoScriptingDriver::Shutdown() {
        mono_jit_cleanup(s_root_domain);
    }

    Object *MonoScriptingDriver::GetNativeObject(MonoObject *object) {
        auto it = s_objects.find(object);
        if (it != s_objects.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    bool MonoScriptingDriver::IsRegisterdObject(MonoObject *object) {
        return s_objects.find(object) != s_objects.end();
    }

    void MonoScriptingDriver::RegisterObject(MonoObject *managed_object, Object *native_object) {
        HYP_ASSERT(s_objects.find(managed_object) == s_objects.end());
        s_objects[managed_object] = native_object;
    }

    void MonoScriptingDriver::UnregisterObject(MonoObject *managed_object) {
        HYP_ASSERT(s_objects.find(managed_object) != s_objects.end());
        s_objects.erase(managed_object);
    }

    void MonoScriptingDriver::InitDebugger(const ScriptingSettings &settings) {
        mono_debug_init(MONO_DEBUG_FORMAT_MONO);

        String debug_option;
        if (settings.wait_for_debugger) {          
            debug_option = StringUtils::Format("--debugger-agent=transport=dt_socket,address=127.0.0.1:{},server=y,embedding=1,server=y,suspend=y,timeout={}", settings.debugger_port, settings.debugger_wait_timeout);
        } else {
            debug_option = StringUtils::Format("--debugger-agent=transport=dt_socket,address=127.0.0.1:{},server=y,embedding=1,server=y,suspend=n", settings.debugger_port);
        }

        const char *options[2] = {
            debug_option.c_str(),
            "--soft-breakpoints"
        };
        mono_jit_parse_options(2, reinterpret_cast<char **>(&options));
    }

    void MonoScriptingDriver::InitDomain() {
        mono_set_dirs("data/mono/lib", "data/mono/etc");
        s_root_domain = mono_jit_init_version("Hyperion", "v4.0.30319");
        if (!s_root_domain) {
            HYP_LOG_ERROR("Scripting", "Initialization of app domain failed!");
            return;
        } else {
            HYP_LOG_INFO("Scripting", "Initialized Mono scripting driver!");
        }
        mono_thread_set_main(mono_thread_current());
    }

    void MonoScriptingDriver::InitBindings() {
        MonoBindingsEntity::Bind();
        MonoBindingsInput::Bind();
        MonoBindingsObject::Bind();
        MonoBindingsTime::Bind();
    }

    void MonoScriptingDriver::InitAssembly() {
        char *assembly_path = "data/managed/Hyperion.Core.dll";
        s_core_assembly = mono_domain_assembly_open(s_root_domain, assembly_path);
        HYP_ASSERT(s_core_assembly);
        s_core_assembly_image = mono_assembly_get_image(s_core_assembly);
        HYP_ASSERT(s_core_assembly_image);
    }

    void MonoScriptingDriver::InitMethods() {
        MonoMethodDesc *start_method_description = mono_method_desc_new("Hyperion.Application::Start()", true);
        MonoMethod *start_method = mono_method_desc_search_in_image(start_method_description, s_core_assembly_image);
        mono_runtime_invoke(start_method, nullptr, nullptr, nullptr);

        MonoMethodDesc *update_method_description = mono_method_desc_new("Hyperion.Application::Update()", true);
        s_update_method = mono_method_desc_search_in_image(update_method_description, s_core_assembly_image);
    }

}