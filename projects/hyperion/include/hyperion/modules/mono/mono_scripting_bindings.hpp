#pragma once

namespace Hyperion::Scripting {

    class MonoScriptingBindings final {
    public:
        static void Bind();
    private:
        MonoScriptingBindings() = delete;
        ~MonoScriptingBindings() = delete;
    };

}