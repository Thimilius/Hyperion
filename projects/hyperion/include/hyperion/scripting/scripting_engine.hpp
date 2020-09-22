#pragma once

#include <mono/jit/jit.h>

namespace Hyperion {
    class Engine;
}

namespace Hyperion {

    class ScriptingEngine {
    public:
        static void Init();
        static void Shutdown();
    private:
        ScriptingEngine() = delete;
        ~ScriptingEngine() = delete;
    private:
        inline static MonoDomain *s_root_domain;
    private:
        friend class Hyperion::Engine;
    };

}