#pragma once

#include <mono/jit/jit.h>

namespace Hyperion {

    class ScriptingEngine {
    private:
        inline static MonoDomain *s_root_domain;
    public:
        static void Init();
        static void Shutdown();
    private:
        ScriptingEngine() = delete;
        ~ScriptingEngine() = delete;

        friend class Engine;
    };

}