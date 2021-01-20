#pragma once

#include <mono/jit/jit.h>

#include "hyperion/scripting/scripting_driver.hpp"

namespace Hyperion {

    class MonoScriptingDriver : public IScriptingDriver {
    public:
        void Init() override;
        void Shutdown() override;
    private:
        void TestFunctions();
    private:
        MonoDomain *m_root_domain;
    };

}