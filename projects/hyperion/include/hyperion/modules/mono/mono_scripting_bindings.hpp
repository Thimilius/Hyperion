#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoScriptingBindings final {
    public:
        static void Bind();
        static void RegisterClasses();
    private:
        MonoScriptingBindings() = delete;
        ~MonoScriptingBindings() = delete;
    };

}