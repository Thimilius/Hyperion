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
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/system/engine.hpp"
#include "hyperion/modules/mono/mono_scripting_bindings.hpp"
#include "hyperion/modules/mono/mono_scripting_storage.hpp"
#include "hyperion/modules/mono/managed/mono_managed_string.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    uint64 MonoScriptingDriver::GetMemoryUsage() const {
        return mono_gc_get_used_size();
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::Initialize(const ScriptingSettings &settings) {
        s_settings = settings;

        InitializeDebugger(settings);
        InitializeDomain();
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::PostInitialize() {
        if (IsInRuntime()) {
            s_engine_methods.initialize_method.Invoke(nullptr, nullptr);
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::EngineModeChange(EngineMode engine_mode) {
        if (engine_mode == EngineMode::EditorRuntime) {
            ReloadRuntimeDomain();

            s_engine_methods.initialize_method.Invoke(nullptr, nullptr);
        } else if (engine_mode == EngineMode::Editor) {
            s_engine_methods.shutdown_method.Invoke(nullptr, nullptr);
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::Update() {
        if (!IsInRuntime()) {
            return;
        }

        float32 delta_time = Time::GetDeltaTime();
        void *parameters[] = { &delta_time };
        s_engine_methods.update_method.Invoke(nullptr, parameters);
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::FixedUpdate() {
        if (!IsInRuntime()) {
            return;
        }

        float32 fixed_delta_time = Time::GetFixedDeltaTime();
        void *parameters[] = { &fixed_delta_time };
        s_engine_methods.fixed_update_method.Invoke(nullptr, parameters);
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::Shutdown() {
        if (IsInRuntime()) {
            s_engine_methods.shutdown_method.Invoke(nullptr, nullptr);
        }

        // We do not need to bother explicitly unloading the runtime domain.
        s_domain_root.SetActive();
        s_domain_root.Finalize();
        mono_jit_cleanup(s_domain_root.GetMonoDomain());
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::InitializeDebugger(const ScriptingSettings &settings) {
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
    void MonoScriptingDriver::InitializeDomain() {
        mono_set_dirs((s_settings.runtime_path + "lib").c_str(), (s_settings.runtime_path + "etc").c_str());
        mono_set_assemblies_path((s_settings.runtime_path + "lib/mono/4.5").c_str());

        MonoDomain *root_domain = mono_jit_init_version("Hyperion.RootDomain", "v4.0.30319");
        if (!root_domain) {
            HYP_LOG_ERROR("Scripting", "Initialization of app domain failed!");
            return;
        } else {
            HYP_LOG_INFO("Scripting", "Initialized Mono scripting driver!");
        }
        s_domain_root = MonoManagedDomain(root_domain);
        MonoManagedDomain::SetCurrentMainThread();

        ReloadRuntimeDomain();
    }

    //--------------------------------------------------------------
    void MonoScriptingDriver::ReloadRuntimeDomain() {
        // We have to clean up the old domain 
        if (s_domain_runtime.GetMonoDomain() != nullptr) {
            // Before unloading the runtime domain, we have to remember to set the root domain active.
            s_domain_root.SetActive();
            s_domain_runtime.Unload();
        }

        s_domain_runtime = MonoManagedDomain::Create("Hyperion.RuntimeDomain");
        s_domain_runtime.SetActive();

        s_assembly_core = s_domain_runtime.LoadAssembly((s_settings.library_path + "Hyperion.Core.dll").c_str());
        s_assembly_editor = s_domain_runtime.LoadAssembly((s_settings.library_path + "Hyperion.Editor.dll").c_str());

        s_engine_methods.initialize_method = s_assembly_core.FindMethod("Hyperion.Engine:Initialize()");
        s_engine_methods.update_method = s_assembly_core.FindMethod("Hyperion.Engine:Update(single)");
        s_engine_methods.fixed_update_method = s_assembly_core.FindMethod("Hyperion.Engine:FixedUpdate(single)");
        s_engine_methods.shutdown_method = s_assembly_core.FindMethod("Hyperion.Engine:Shutdown()");

        MonoScriptingStorage::Clear();
        MonoScriptingBindings::RegisterClasses();
        MonoScriptingBindings::Bind();

        // We have to trigger the internal registration.
        s_assembly_core.FindMethod("Hyperion.Engine:Register()").Invoke(nullptr, nullptr);
    }

    //--------------------------------------------------------------
    bool MonoScriptingDriver::IsInRuntime() {
        return Engine::GetMode() == EngineMode::Runtime || Engine::GetMode() == EngineMode::EditorRuntime;
    }

}