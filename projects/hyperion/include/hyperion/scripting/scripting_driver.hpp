#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/system/engine_mode.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class IScriptingDriver {
    public:
        virtual ~IScriptingDriver() = default;
    public:
        virtual uint64 GetMemoryUsage() const = 0;
        virtual void EngineModeChange(EngineMode engine_mode) = 0;

        virtual void Initialize(const ScriptingSettings &settings) = 0;
        virtual void Update() = 0;
        virtual void Shutdown() = 0;
    };

    class DummyScriptingDriver final : public IScriptingDriver {
    public:
        uint64 GetMemoryUsage() const override { return 0; }
        void EngineModeChange(EngineMode engine_mode) override { }

        void Initialize(const ScriptingSettings &settings) override { HYP_LOG_INFO("Scripting", "Initialized dummy scripting driver!"); }
        void Update() override { }
        void Shutdown() override { }
    };

}