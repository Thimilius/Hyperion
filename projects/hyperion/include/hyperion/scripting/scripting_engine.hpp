#pragma once

#include <mono/jit/jit.h>

namespace Hyperion {
    class Engine;
}

namespace Hyperion {

    class ScriptingEngine {
    private:
        ScriptingEngine() = delete;
        ~ScriptingEngine() = delete;

        static void Init();
        static void Shutdown();

        static void TestFunctions();
    private:
        inline static MonoDomain *s_root_domain;
    private:
        friend class Hyperion::Engine;
    };

}