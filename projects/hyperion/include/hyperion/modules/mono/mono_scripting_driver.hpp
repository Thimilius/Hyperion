#pragma once

#include <mono/jit/jit.h>

#include "hyperion/core/object/object.hpp"
#include "hyperion/scripting/scripting_driver.hpp"

namespace Hyperion {

    class MonoScriptingDriver : public IScriptingDriver {
    public:
        void Initialize(const ScriptingSettings &settings) override;
        void Shutdown() override;

        inline static MonoDomain *GetDomain() { return s_root_domain; }

        static Object *GetNativeObject(MonoObject *object);
        static bool IsRegisterdObject(MonoObject *object);
        static void RegisterObject(MonoObject *managed_object, Object *native_object);
        static void UnregisterObject(MonoObject *managed_object);
    private:
        void InitDebugger(const ScriptingSettings &settings);
        void InitDomain();

        void InitBindings();
        void TestFunctions();
    private:
        inline static MonoDomain *s_root_domain;

        inline static Map<MonoObject *, Object *> s_objects;
    };

}