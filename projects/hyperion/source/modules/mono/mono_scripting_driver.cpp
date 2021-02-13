//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/mono_scripting_driver.hpp"

//---------------------- Library Includes ----------------------
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/threads.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/engine.hpp"
#include "hyperion/modules/mono/mono_scripting_bindings.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    uint64 MonoScriptingDriver::GetMemoryUsage() const {
        return mono_gc_get_used_size();
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::Initialize(const ScriptingSettings &settings) {
        InitDebugger(settings);
        InitDomain();
        InitAssembly();
        InitBindings();
        InitClasses();
        InitMethods();
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::EngineModeChange(EngineMode engine_mode) {
        if (engine_mode == EngineMode::Editor) {
            // We take a copy here as the actual vector gets changed, so we can not iterate over it directly.
            Vector<Script *> scripts = s_scripts_which_recieve_messages;
            for (Script *script : scripts) {
                script->GetScriptingInstance()->SendMessage(ScriptingMessage::OnDestroy);
            }
        } else if (engine_mode == EngineMode::EditorRuntime) {
            // Recreate all script components.
            //for (Script *script : s_scripts_which_recieve_messages) {
            //    CreateManagedObjectFromManagedType(script, static_cast<MonoScriptingType *>(script->GetScriptingType())->GetManagedClass(), true);
            //}

            for (Script *script : s_scripts_which_recieve_messages) {
                script->GetScriptingInstance()->SendMessage(ScriptingMessage::OnCreate);
            }
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::Update() {
        if (Engine::GetMode() == EngineMode::EditorRuntime) {
            for (Script *script : s_scripts_which_recieve_messages) {
                script->GetScriptingInstance()->SendMessage(ScriptingMessage::OnUpdate);
            }
        }

        MonoObject *exception = nullptr;
        mono_runtime_invoke(s_editor_update_method, nullptr, nullptr, &exception);
        if (exception != nullptr) {
            PrintUnhandledException(exception);
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::Shutdown() {
        for (auto [managed_class, scripting_type] : s_scripting_types) {
            delete scripting_type;
        }

        mono_jit_cleanup(s_core_domain);
    }

    //--------------------------------------------------------------
    void *MonoScriptingDriver::GetNativeObject(MonoObject *managed_object) {
        auto it = s_managed_to_native_objects.find(managed_object);
        if (it != s_managed_to_native_objects.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    MonoObject *MonoScriptingDriver::GetOrCreateManagedObject(Object *native_object, Type native_type) {
        MonoScriptingInstance *scripting_instance = static_cast<MonoScriptingInstance *>(native_object->GetScriptingInstance());
        if (scripting_instance) {
            return scripting_instance->GetManagedObject();
        } else {
            return CreateManagedObjectFromNativeType(native_object, native_type);
        }
    }

    //--------------------------------------------------------------
    MonoObject *MonoScriptingDriver::GetOrCreateManagedObjectRaw(void *native, Type native_type) {
        HYP_ASSERT(s_native_to_managed_classes.find(native_type) != s_native_to_managed_classes.end());

        auto it = s_native_to_managed_objects.find(native);
        if (it != s_native_to_managed_objects.end()) {
            return it->second;
        } else {
            MonoObject *managed_object = mono_object_new(s_core_domain, s_native_to_managed_classes[native_type]);
            RegisterObject(managed_object, native);
            return managed_object;
        }
    }

    //--------------------------------------------------------------
    MonoObject *MonoScriptingDriver::CreateManagedObjectFromManagedType(Object *native_object, MonoClass *managed_class, bool is_script_component) {
        MonoObject *managed_object = mono_object_new(s_core_domain, managed_class);
        RegisterManagedObject(managed_object, native_object, is_script_component);
        return managed_object;
    }

    //--------------------------------------------------------------
    MonoObject *MonoScriptingDriver::CreateManagedObjectFromNativeType(Object *native_object, Type native_type) {
        HYP_ASSERT(s_native_to_managed_classes.find(native_type) != s_native_to_managed_classes.end());
        return CreateManagedObjectFromManagedType(native_object, s_native_to_managed_classes[native_type], false);
    }

    //--------------------------------------------------------------
    bool MonoScriptingDriver::IsRegisterdObject(MonoObject *managed_object) {
        return s_managed_to_native_objects.find(managed_object) != s_managed_to_native_objects.end();
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::RegisterManagedObject(MonoObject *managed_object, Object *native_object, bool is_script_component) {
        RegisterObject(managed_object, native_object);
        MonoScriptingInstance *scripting_instance = new MonoScriptingInstance(managed_object, is_script_component);
        native_object->SetScriptingInstance(scripting_instance);
        if (is_script_component) {
            Script *script = static_cast<Script *>(native_object);
            s_scripts_which_recieve_messages.push_back(script);
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::RegisterObject(MonoObject *managed_object, void *native) {
        HYP_ASSERT(s_managed_to_native_objects.find(managed_object) == s_managed_to_native_objects.end());

        s_managed_to_native_objects[managed_object] = native;
        s_native_to_managed_objects[native] = managed_object;
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::UnregisterObject(MonoObject *managed_object, bool is_script_component) {
        HYP_ASSERT(s_managed_to_native_objects.find(managed_object) != s_managed_to_native_objects.end());

        void *native = s_managed_to_native_objects[managed_object];
        s_managed_to_native_objects.erase(managed_object);
        s_native_to_managed_objects.erase(native);

        if (is_script_component && Engine::GetMode() == EngineMode::Runtime) {
            Script *script = reinterpret_cast<Script *>(native);
            auto begin = s_scripts_which_recieve_messages.begin();
            auto end = s_scripts_which_recieve_messages.end();
            if (std::find(begin, end, script) != end) {
                s_scripts_which_recieve_messages.erase(std::remove(begin, end, script));
            }
        }
    }

    //--------------------------------------------------------------
    bool MonoScriptingDriver::IsNativeClass(MonoClass *native_class) {
        return s_managed_to_native_classes.find(native_class) != s_managed_to_native_classes.end();
    }

    //--------------------------------------------------------------
    Type MonoScriptingDriver::GetNativeClass(MonoClass *native_class) {
        HYP_ASSERT(IsNativeClass(native_class));
        return s_managed_to_native_classes[native_class].get_value<Type>();
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::RegisterClass(Type native_class, const char *managed_namespace, const char *managed_name) {
        MonoClass *managed_class = mono_class_from_name(s_core_assembly_image, managed_namespace, managed_name);
        HYP_ASSERT(managed_class);

        s_native_to_managed_classes[native_class] = managed_class;
        s_managed_to_native_classes[managed_class] = native_class;
    }

    //--------------------------------------------------------------
    MonoScriptingType *MonoScriptingDriver::GetOrCreateScriptingType(MonoClass *managed_class) {
        auto it = s_scripting_types.find(managed_class);
        if (it != s_scripting_types.end()) {
            return it->second;
        } else {
            MonoScriptingType *mono_scripting_type = new MonoScriptingType(managed_class);
            s_scripting_types[managed_class] = mono_scripting_type;
            return mono_scripting_type;
        }
    }

    //--------------------------------------------------------------
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

    //--------------------------------------------------------------
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

    //--------------------------------------------------------------
    void MonoScriptingDriver::InitBindings() {
        MonoScriptingBindings::Bind();
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::InitAssembly() {
        {
            char *assembly_path = "data/managed/Hyperion.Core.dll";
            s_core_assembly = mono_domain_assembly_open(s_core_domain, assembly_path);
            HYP_ASSERT(s_core_assembly);
            s_core_assembly_image = mono_assembly_get_image(s_core_assembly);
            HYP_ASSERT(s_core_assembly_image);
        }
        {
            char *assembly_path = "data/managed/Hyperion.Editor.dll";
            s_editor_assembly = mono_domain_assembly_open(s_core_domain, assembly_path);
            HYP_ASSERT(s_editor_assembly);
            s_editor_assembly_image = mono_assembly_get_image(s_editor_assembly);
            HYP_ASSERT(s_editor_assembly_image);
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::InitClasses() {
        s_component_class = mono_class_from_name(s_core_assembly_image, "Hyperion", "Component");
        HYP_ASSERT(s_component_class);

        s_script_class = mono_class_from_name(s_core_assembly_image, "Hyperion", "Script");
        HYP_ASSERT(s_script_class);
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::InitMethods() {
        MonoMethodDesc *start_method_description = mono_method_desc_new("Hyperion.Editor.Application::Start()", true);
        MonoMethod *start_method = mono_method_desc_search_in_image(start_method_description, s_editor_assembly_image);
        mono_runtime_invoke(start_method, nullptr, nullptr, nullptr);

        MonoMethodDesc *update_method_description = mono_method_desc_new("Hyperion.Editor.Application::Update()", true);
        s_editor_update_method = mono_method_desc_search_in_image(update_method_description, s_editor_assembly_image);
    }

    //--------------------------------------------------------------
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

}