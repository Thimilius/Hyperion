#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class IScriptingDriver {
    public:
        virtual ~IScriptingDriver() = default;
    public:
        virtual void Initialize(const ScriptingSettings &settings) = 0;
        virtual void Shutdown() = 0;
    };

    class DummyScriptingDriver : public IScriptingDriver {
    public:
        void Initialize(const ScriptingSettings &settings) override { HYP_LOG_INFO("Scripting", "Initialized dummy scripting driver!"); }
        void Shutdown() override { }
    };

}