#include "hyppch.hpp"

#include "hyperion/modules/mono/mono_scripting_driver.hpp"

#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/mono-debug.h>

#include "hyperion/modules/mono/bindings/mono_bindings_object.hpp"

namespace Hyperion {

    void MonoScriptingDriver::Init() {
        mono_set_dirs("data/mono/lib", "data/mono/etc");

        s_root_domain = mono_jit_init("Hyperion");
        if (!s_root_domain) {
            HYP_LOG_ERROR("Scripting", "Initialization of app domain failed!");
            return;
        } else {
            HYP_LOG_INFO("Scripting", "Initialized Mono scripting driver!");
        }

        SetupBindings();

        TestFunctions();
    }

    void MonoScriptingDriver::Shutdown() {
        mono_jit_cleanup(s_root_domain);
    }

    Object *MonoScriptingDriver::GetNativeObject(MonoObject *object) {
        HYP_ASSERT(s_objects.find(object) != s_objects.end());
        return s_objects[object];
    }

    bool MonoScriptingDriver::IsRegisterdObject(MonoObject *object) {
        return s_objects.find(object) != s_objects.end();
    }

    void MonoScriptingDriver::RegisterObject(MonoObject *managed_object, Object *native_object) {
        HYP_ASSERT(s_objects.find(managed_object) == s_objects.end());
        s_objects[managed_object] = native_object;
    }

    void MonoScriptingDriver::UnregisterObject(MonoObject *managed_object) {
        HYP_ASSERT(s_objects.find(managed_object) != s_objects.end());
        s_objects.erase(managed_object);
    }

    void MonoScriptingDriver::SetupBindings() {
        MonoBindingsObject::Bind();
    }

    void MonoScriptingDriver::TestFunctions() {
        char *assembly_path = "data/managed/Hyperion.dll";
        MonoAssembly *assembly = mono_domain_assembly_open(s_root_domain, assembly_path);
        MonoImage *image = mono_assembly_get_image(assembly);

        MonoMethodDesc *description = mono_method_desc_new("Hyperion.Application::Run()", true);
        MonoMethod *method = mono_method_desc_search_in_image(description, image);

        mono_runtime_invoke(method, nullptr, nullptr, nullptr);
    }

}