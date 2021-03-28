#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/object/object.hpp"
#include "hyperion/entity/components/script.hpp"
#include "hyperion/modules/mono/mono_scripting_instance.hpp"
#include "hyperion/modules/mono/managed/mono_managed_assembly.hpp"
#include "hyperion/modules/mono/managed/mono_managed_domain.hpp"
#include "hyperion/modules/mono/managed/mono_managed_method.hpp"
#include "hyperion/scripting/scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {
    
    class MonoScriptingDriver : public IScriptingDriver {
    public:
        uint64 GetMemoryUsage() const override;

        void Initialize(const ScriptingSettings &settings) override;
        void PostInitialize() override;
        void EngineModeChange(EngineMode engine_mode) override;
        void Update() override;
        void FixedUpdate() override;
        void Shutdown() override;
    public:
        inline static MonoManagedDomain *GetRuntimeDomain() { return &s_domain_runtime; }
    private:
        static void InitializeDebugger(const ScriptingSettings &settings);
        static void InitializeDomain();

        static void ReloadRuntimeDomain();

        static bool IsInRuntime();
    private:
        inline static ScriptingSettings s_settings;

        inline static MonoManagedDomain s_domain_root;
        inline static MonoManagedDomain s_domain_runtime;

        inline static MonoManagedAssembly s_assembly_core;
        inline static MonoManagedAssembly s_assembly_editor;

        inline static struct EngineMethods {
            MonoManagedMethod initialize_method;
            MonoManagedMethod update_method;
            MonoManagedMethod fixed_update_method;
            MonoManagedMethod shutdown_method;
        } s_engine_methods;
    };

}