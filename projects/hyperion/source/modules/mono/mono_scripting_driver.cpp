#include "hyppch.hpp"

#include "hyperion/modules/mono/mono_scripting_driver.hpp"

#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/threads.h>

#include "hyperion/modules/mono/mono_scripting_bindings.hpp"

namespace Hyperion::Scripting {

    void MonoScriptingDriver::Initialize(const ScriptingSettings &settings) {
        InitDebugger(settings);
        InitDomain();
        InitAssembly();
        InitBindings();
        InitMethods();
    }

    void MonoScriptingDriver::Update() {
        MonoObject *exception = nullptr;
        mono_runtime_invoke(s_update_method, nullptr, nullptr, &exception);
        if (exception != nullptr) {
            PrintUnhandledException(exception);
        }
    }

    void MonoScriptingDriver::Shutdown() {
        mono_jit_cleanup(s_core_domain);
    }

    Object *MonoScriptingDriver::GetNativeObject(MonoObject *managed_object) {
        auto it = s_native_objects.find(managed_object);
        if (it != s_native_objects.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    MonoObject *MonoScriptingDriver::GetManagedObject(Object *native_object) {
        auto it = s_managed_objects.find(native_object);
        if (it != s_managed_objects.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    MonoObject *MonoScriptingDriver::GetOrCreateManagedObject(Object *native_object, Type native_type) {
        MonoObject *managed_object = GetManagedObject(native_object);
        if (!managed_object) {
            managed_object = CreateManagedObject(native_type, native_object);
        }
        return managed_object;
    }

    MonoObject *MonoScriptingDriver::CreateManagedObject(Type type, Object *native_object) {
        HYP_ASSERT(s_managed_classes.find(type) != s_managed_classes.end());

        MonoObject *managed_object = mono_object_new(s_core_domain, s_managed_classes[type]);
        RegisterObject(managed_object, native_object);
        return managed_object;
    }

    bool MonoScriptingDriver::IsRegisterdObject(MonoObject *managed_object) {
        return s_native_objects.find(managed_object) != s_native_objects.end();
    }

    void MonoScriptingDriver::RegisterObject(MonoObject *managed_object, Object *native_object) {
        HYP_ASSERT(s_native_objects.find(managed_object) == s_native_objects.end());

        s_native_objects[managed_object] = native_object;
        s_managed_objects[native_object] = managed_object;
    }

    void MonoScriptingDriver::UnregisterObject(MonoObject *managed_object) {
        HYP_ASSERT(s_native_objects.find(managed_object) != s_native_objects.end());

        Object *native_object = s_native_objects[managed_object];
        s_native_objects.erase(managed_object);
        s_managed_objects.erase(native_object);
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
        s_core_domain = mono_jit_init_version("Hyperion", "v4.0.30319");
        if (!s_core_domain) {
            HYP_LOG_ERROR("Scripting", "Initialization of app domain failed!");
            return;
        } else {
            HYP_LOG_INFO("Scripting", "Initialized Mono scripting driver!");
        }
        mono_thread_set_main(mono_thread_current());
    }

    void MonoScriptingDriver::InitBindings() {
        MonoScriptingBindings::Bind();
    }

    void MonoScriptingDriver::InitAssembly() {
        char *assembly_path = "data/managed/Hyperion.Core.dll";
        s_core_assembly = mono_domain_assembly_open(s_core_domain, assembly_path);
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

    void MonoScriptingDriver::PrintUnhandledException(MonoObject *exception) {
        HYP_ASSERT(exception);

        MonoClass *exception_class = mono_object_get_class(exception);

        MonoProperty *message_property = mono_class_get_property_from_name(mono_get_exception_class(), "Message");
        MonoObject *managed_message = mono_property_get_value(message_property, exception, nullptr, nullptr);
        char *message = mono_string_to_utf8(reinterpret_cast<MonoString *>(managed_message));

        MonoProperty *stack_trace_property = mono_class_get_property_from_name(mono_get_exception_class(), "StackTrace");
        MonoObject *managed_stack_trace = mono_property_get_value(stack_trace_property, exception, nullptr, nullptr);
        char *stack_trace = mono_string_to_utf8(reinterpret_cast<MonoString *>(managed_stack_trace));

        HYP_LOG_ERROR("Scripting", "{}: {}\n{}", mono_class_get_name(exception_class), message, stack_trace);

        mono_free(message);
        mono_free(stack_trace);
    }

    Type MonoScriptingDriver::GetNativeClass(MonoClass *native_class) {
        HYP_ASSERT(s_native_classes.find(native_class) != s_native_classes.end());
        return s_native_classes[native_class].get_value<Type>();
    }

    void MonoScriptingDriver::RegisterClass(Type native_class, const char *managed_namespace, const char *managed_name) {
        MonoClass *managed_class = mono_class_from_name(s_core_assembly_image, managed_namespace, managed_name);
        HYP_ASSERT(managed_class);

        s_managed_classes[native_class] = managed_class;
        s_native_classes[managed_class] = native_class;
    }

}